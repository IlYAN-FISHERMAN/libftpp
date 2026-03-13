/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 13:40:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hh"

void lpp::server::_workerLoop(){
	{
		std::lock_guard<std::mutex> lock(_mutex);
		try{
			cout.setPrefix("[server]: ");
			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (_socket < 0)
				throw std::runtime_error("socket failed");
			cout << "started..." << std::endl;
			_serv_addr.sin_family = AF_INET;
			_serv_addr.sin_addr.s_addr = INADDR_ANY;
			_serv_addr.sin_port = htons(_p_port);
			if (bind(_socket, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
				std::runtime_error("bind failed");
			else if (::listen(_socket, 5) < 0)
				throw std::runtime_error("listen failed");
			_pollFd.push_back(pollfd{_socket, POLLIN, 0});
			_msg.push_back("");
			cout << "listening..." << std::endl;
			_running.store(true);
		} catch (...){
			_exc = std::current_exception();
			return;
		}
	}
	while (_running.load()){
		if (poll(_pollFd.data(), _pollFd.size(), 100) < 0){
			break;
		}
		if (!_running.load()){
			cout << "stopping server..." << std::endl;
			break ;
		}

		for(size_t i = 0; i < _pollFd.size(); ++i){
			if (_pollFd[i].revents & POLLIN){
				if (_pollFd[i].fd == _socket){
					int acpt = accept(_socket, 0x00, 0x00);
					if (acpt >= 0){
						_pollFd.push_back(pollfd{acpt, POLLIN, 0});
						_msg.push_back("");
						cout << "new client connected" << std::endl;
					}
					continue;
				}
				char buffer[BUFFER_SIZE]{0};
				ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
				cout << "read " << n << " bytes" << std::endl;
				if (n <= 0){
					cout << "close client" << std::endl;
					close(_pollFd[i].fd);
					_pollFd.erase(_pollFd.begin() + i);
					_msg.erase(_msg.begin() + i);
					i--;
				} else{
					_msg[i].append(buffer, n);
					std::stringstream ss(_msg[i]);
					int code = 0;
					char sep = 0;
					if (ss >> code >> sep){
						message msg(code);
						if (_actions.find(code) != _actions.end() && sep == '|'){
							std::string tmp;
							std::getline(ss >> std::ws, tmp);
							msg << tmp;
							_actions[code](_pollFd[i].fd, msg);
						}
						else{
							lpp::cout << "send error: " << ::send(_pollFd[i].fd, "action not found", 15, 0) << std::endl;
						}
					}
					else{
						::send(_pollFd[i].fd, _msg[i].c_str(), _msg[i].size(), 0);
					}
					_msg[i].clear();
				}
			}
		}
	}
	return ;
}

void lpp::server::start(const size_t& p_port){
	if (!_running.load()){
		_p_port = p_port;
		_loop = std::thread(&server::_workerLoop, this);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (!_running.load() && _exc){
		this->disconnect();
		std::rethrow_exception(_exc);
	}
}

void lpp::server::disconnect(){
	if (_running.load()){
		_running.store(false);
		if(_loop.joinable())
			_loop.join();
	}
	close(_socket);
}

lpp::server::~server(){
	disconnect();
}

lpp::server::server() : _exc(nullptr){
	_running.store(false);
}

void lpp::server::defineAction(const message::Type& messageType, const std::function<void(long long clientID, const message& msg)>& action){
	std::lock_guard<std::mutex> lock(_mutex);
	_actions[messageType] = action;
}

void lpp::server::sendTo(const message& message, long long clientID){
	std::string data = (std::to_string(message.type()) + '|' + message.str() + '\n');
	lpp::cout << "message type: " << message.type() << std::endl;
	lpp::cout << "send to client[" << clientID << "] "
		<< ::send(clientID, data.c_str(), data.size(), 0) << " bytes" << std::endl;
}

bool lpp::server::config(){
	// Default action configuration
    defineAction(1, [&](long long clientID, const lpp::message& msg){
        int value;
        msg >> value;
        lpp::cout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 1 with double the value
        lpp::message replyMsg(1);
        replyMsg << (value * 2);
        sendTo(replyMsg, clientID);
    });

    defineAction(10, [&](long long clientID, const lpp::message& msg){
		std::string value;
        lpp::message replyMsg(10);

        msg >> value;
		if (value != "ping")
			replyMsg << "ping?";
		else{
			replyMsg << "pong";
		}

        sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    defineAction(2, [&](long long clientID, const lpp::message& msg){
        size_t length;
        std::string text;
        msg >> length;
        text.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            char c;
            msg >> c;
            text.push_back(c);
        }
        lpp::cout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
		lpp::message tmp(2);
		tmp << "message received";
		sendTo(tmp, clientID);
    });

	return true;
}

bool lpp::server::execute(){
	if (!_running.load())
		start(8080);
	return true;
}
