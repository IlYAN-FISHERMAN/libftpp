/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:02 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/27 13:37:26 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hh"

lpp::client::client() : _socket(-1), _running(false), _chrono("lpp::client"){
	_logger.setIsStdout(true);
}

lpp::client::~client(){
	close(_socket);
}

void lpp::client::connect(const std::string& address, const size_t& port){
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_host = address;
	if (_socket < 0)
		throw std::runtime_error("socket failed");
	
	sockaddr_in	addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (inet_pton(AF_INET, _host.c_str(), &addr.sin_addr) <= 0)
            throw std::runtime_error("invalid address");
	if (::connect(_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
            throw std::runtime_error("connection failed");
	_running = true;
}

void lpp::client::disconnect(){
	if (_socket > 0)
		close(_socket);
	_socket = -1;
}

void lpp::client::defineAction(const message::Type& messageType, const std::function<void(const message& msg)>& action){
	_actions[messageType] = action;
}

std::string lpp::client::received(std::string response){
	int code = 0;
	char sep = 0;
	std::stringstream ss(response);
	if (ss.str().find('|') == std::string::npos)
		return ss.str();
	else if (ss >> code >> sep && sep == '|'){
		if (_actions.find(code) != _actions.end()){
			message msg(code);
			std::string reply;
			std::getline(ss >> std::ws, reply);
			msg << reply;
			_actions[code](msg);
			return reply;
		} else
			return "action not found";
	}else
		return "action bad format";

	return "received failed";
}

std::string lpp::client::send(const message& msg){
	std::string data = (std::to_string(msg.type()) + '|' + msg.str());
	size_t send = ::send(_socket, data.c_str(), data.size(), 0);
	_logger.log(INFO, "send " + std::to_string(send) + " bytes");
	char buffer[1024]{0};
	ssize_t n = read(_socket, buffer, sizeof(buffer));
	if (n > 0)
		return received(std::string(buffer, n));
	return "send failed: " + std::to_string(n);
}

std::string lpp::client::send(std::string msg, bool print){
	_logger.log(INFO, "send " + std::to_string(::send(_socket, msg.c_str(), msg.size(), 0)) + " bytes");
	char buffer[1024];
	ssize_t n = read(_socket, buffer, sizeof(buffer));
	if (n > 0) {
		if (print){
			std::string reply(buffer, n);
			lpp::cout << "server reponse: " << reply << std::endl;
		}
	} else{
		if (print)
			lpp::cout << "send failed: " << n << std::endl;
		else
			return ("send failed");
	}
	
	return std::string(buffer, n);
}

std::string lpp::client::send(const char *msg, size_t size){
	if (::send(_socket, msg, size, 0) < 0)
		return ("send failed");
	char buffer[1024];

	ssize_t n = read(_socket, buffer, sizeof(buffer));
	if (n > 0)
		return (std::string(buffer, n));
	return ("read failed");
}

bool lpp::client::config(){
	defineAction(1, [](const lpp::message& msg){
        int doubledValue;
		msg >> doubledValue;
        lpp::cout << "Received a doubled value: " << doubledValue << std::endl;
    });

    defineAction(2, [](const lpp::message& msg){
		lpp::cout << "server say \"" << msg.str() << "\"" << std::endl;
    });

	defineAction(4, [](const lpp::message& msg){
		std::string str;
        msg >> str;
        lpp::cout << "client received : " << str << std::endl;
    });

	defineAction(10, [](const lpp::message& msg){
		std::string str;
        msg >> str;
		lpp::cout << str << std::endl;
		if (str == "pong")
			lpp::cout << "server work!" << std::endl;
    });

	return true;
}

bool lpp::client::execute(){
	if (!_running)
		connect("127.0.0.1", 8080);
	return true;
}

bool lpp::client::isRunning(){return _running;}
lpp::logger& lpp::client::getLogger(){return _logger;}
