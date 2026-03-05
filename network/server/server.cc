/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/05 11:45:21 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hh"

static class threadSafeCout cout;

void Server::_workerLoop(){
	{
		std::lock_guard<std::mutex> lock(_mutex);
		cout.setPrefix("[Server]: ");
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket < 0){
			_exc = std::make_exception_ptr("socket failed");
			return;
		}
		cout << "started..." << std::endl;
		_serv_addr.sin_family = AF_INET;
		_serv_addr.sin_addr.s_addr = INADDR_ANY;
		_serv_addr.sin_port = htons(_p_port);
		if (bind(_socket, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0){
			_exc = std::make_exception_ptr("bind failed");
			return ;
		} else if (::listen(_socket, 5) < 0)
			_exc =  std::make_exception_ptr("listen failed");
		_pollFd.push_back(pollfd{_socket, POLLIN, 0});
		_msg.push_back("");
		cout << "listening..." << std::endl;
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
				if (n <= 0){
					cout << "close client" << std::endl;
					close(_pollFd[i].fd);
					_pollFd.erase(_pollFd.begin() + i);
					_msg.erase(_msg.begin() + i);
					i--;
				} else{
					cout << "read data" << std::endl;
					_msg[i].append(buffer, n);
					std::stringstream ss(_msg[i]);
					int code = 0;
					char sep = 0;
					std::string tmp;
					if (ss >> code >> sep >> tmp){
						Message msg(code);
						msg << tmp;
						if (_actions.find(code) != _actions.end())
							_actions[code](_pollFd[i].fd, msg);
					}
					else
						::send(_pollFd[i].fd, _msg[i].c_str(), _msg[i].size(), 0);
					_msg[i].clear();
				}
			}
		}
	}
	return ;
}

void Server::start(const size_t& p_port){
	if (!_running.load()){
		_running.store(true);
		_p_port = p_port;
		_loop = std::thread(&Server::_workerLoop, this);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Server::disconnect(){
	if (_running.load()){
		_running.store(false);
		if(_loop.joinable())
			_loop.join();
	}
	close(_socket);
}

Server::~Server(){
	disconnect();
}

Server::Server(){
	_running.store(false);
}

void Server::defineAction(const Message::Type& messageType, const std::function<void(long long clientID, const Message& msg)>& action){
	std::lock_guard<std::mutex> lock(_mutex);
	_actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID){
	std::string value;
	message >> value;
	std::string data = (std::to_string(message.type()) + '|' + value + '\n');
	threadSafeCout << "message type: " << message.type() << std::endl;
	::send(clientID, data.c_str(), data.size(), 0);
}

// void sendToArray(const Message& message, std::vector<long long> clientIDs){}
//
// void sendToAll(const Message& message){
//
// }

void Server::update(){}
