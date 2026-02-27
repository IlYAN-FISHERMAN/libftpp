/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:13:41 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hh"

static bool sigHandler = false;
static int sigCode = 0;

static void handleSigint(int sig){
	sigHandler = true;
	sigCode = sig;
}

void Server::start(const size_t& p_port){
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket < 0)
		throw std::runtime_error("socket failed");
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_addr.sin_port = htons(p_port);

    if (bind(_socket, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0){
        throw std::runtime_error("bind failed");
	} else if (::listen(_socket, 5) < 0)
		throw std::runtime_error("listen failed");
	std::signal(2, handleSigint);
	std::signal(13, handleSigint);
	std::signal(20, handleSigint);
	std::cout << "Server started..." << std::endl;
	std::cout << "Server listening..." << std::endl;
	while (!sigHandler){
		if (poll(_pollFd.data(), _pollFd.size(), -1) < 0){
			break;
		}
		for(size_t i = 0; i < _pollFd.size(); ++i){
			if (_pollFd[i].revents & POLLIN)
			{
				if (_pollFd[i].fd == _socket){
					int acpt = accept(_socket, 0x00, 0x00);
					if (acpt >= 0){
						_pollFd.push_back(pollfd{acpt, POLLIN, 0});
					}
				}
				else{
					char buffer[BUFFER_SIZE]{0};
					ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
					if (n <= 0){
						close(_pollFd[i].fd);
						_pollFd.erase(_pollFd.begin() + i);
						_msg[i].clear();
						if (i > 0)
							i--;
					}
					else{
						_msg[i].append(buffer, n);
						if (n < BUFFER_SIZE)
							_pollFd[i].events |= POLLOUT;
					}
				}
			}
			else if (_pollFd[i].revents & POLLOUT){
				threadSafeCout << _msg[i] << std::endl;
				_pollFd[i].events &= ~POLLOUT;
			}
		}
	}
	sigHandler = false;
	return ;
}

Server::~Server(){
	close(_socket);
	std::signal(2, SIG_DFL);
	std::signal(13, SIG_DFL);
	std::signal(20, SIG_DFL);
}

Server::Server(){}


void Server::defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action){
	std::lock_guard<std::mutex> lock(_mutex);
	_actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID){
	(void)message;
	(void)clientID;
}
//
// void sendToArray(const Message& message, std::vector<long long> clientIDs){}
//
// void sendToAll(const Message& message){
//
// }

void update(){}
