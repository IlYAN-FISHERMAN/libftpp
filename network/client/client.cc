/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:02 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:26:14 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hh"

Client::Client() : _socket(-1){}

void Client::connect(const std::string& address, const size_t& port){
	int status = 0;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::runtime_error("socket failed");
	
	int flags = fcntl(_socket, F_GETFL, 0);
	if (flags == -1 || fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1){
		close(_socket);
		throw std::runtime_error("Failed to set non-blocking");
	}
	sockaddr_in	addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (address == "localhost") {
		struct addrinfo	hints{}, *res;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		int	status = getaddrinfo("localhost", nullptr, &hints, &res);
		if (status != 0) {
			close(_socket);
			throw std::runtime_error("Failed to resolve localhost");
		}
		addr.sin_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;
		freeaddrinfo(res);
	} else if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
		close(_socket);
		throw std::runtime_error("Invalid address");
	}

	struct hostent *hostp = gethostbyname(address.c_str());
	if (!hostp)
		throw std::runtime_error("gethostbyname("+ address + ")");
	struct sockaddr_in  _serv_addr = {0, 0, 0, {0}, {0}};
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(port);
	_serv_addr.sin_addr = *reinterpret_cast<in_addr*> (hostp->h_addr);

	status = ::connect(_socket, reinterpret_cast<sockaddr*>(&_serv_addr), sizeof(_serv_addr));
	if(status < 0)
        perror("Couldn't connect with the server");
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
	::send(_socket, data.c_str(), data.size(), 0);
}

void Client::update(){
	if (_socket < 0)
		return;
	char buffer[1024];
    int received = recv(_socket, buffer, sizeof(buffer)-1, MSG_DONTWAIT);

	if (received > 0){
		buffer[received] = '\0';
		std::string line(buffer);
		size_t sep = line.find('|');
		if (sep != std::string::npos){
			int typeInt = std::stoi(line.substr(0, sep));
			Message msg(typeInt);
			msg << line.substr(sep+1);
			if (_actions.count(msg.type()))
				_actions[msg.type()](msg);
		}
	}
}
