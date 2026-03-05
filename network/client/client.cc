/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:02 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/05 19:56:59 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hh"

Client::Client() : _socket(-1){}

Client::~Client(){
	close(_socket);
}

void Client::connect(const std::string& address, const size_t& port){
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
}

void Client::disconnect(){
	if (_socket > 0)
		close(_socket);
	_socket = -1;
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action){
	_actions[messageType] = action;
}

void Client::send(const Message& msg){
	if (_socket < 0)
		return ;

	std::string value;
	msg >> value;
	std::string data = (std::to_string(msg.type()) + '|' + value + '\n');
	threadSafeCout << "send " << ::send(_socket, data.c_str(), data.size(), 0) << " bytes" << std::endl;
	char buffer[1024];
	ssize_t n = read(_socket, buffer, sizeof(buffer));
	if (n > 0) {
		int code = 0;
		char sep = 0;
		std::stringstream ss(buffer);
		if (ss.str().find('|') == std::string::npos)
			threadSafeCout << "received: " << ss.str() << std::endl;
		else if (ss >> code >> sep){
			if (sep != '|'){
				threadSafeCout << "action bad format" << std::endl;
				return;
			}
			if (_actions.find(code) != _actions.end()){
				Message msg(code);
				std::string reply;
				std::getline(ss >> std::ws, reply);
				msg << reply;
				_actions[code](msg);
				return;
			} else
				threadSafeCout << "action not found" << std::endl;
		}else
			threadSafeCout << "action bad format" << std::endl;
	}else
		threadSafeCout << "send failed: " << n << std::endl;
}

void Client::send(std::string &msg){
	threadSafeCout << "send " << ::send(_socket, msg.c_str(), msg.size(), 0) << " bytes" << std::endl;
	char buffer[1024];
	ssize_t n = read(_socket, buffer, sizeof(buffer));
	if (n > 0) {
		std::string reply(buffer, n);
		threadSafeCout << "server reponse: " << reply << std::endl;
	} else
		threadSafeCout << "send failed: " << n << std::endl;
}
