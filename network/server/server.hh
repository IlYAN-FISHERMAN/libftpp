/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/05 11:44:01 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../client/client.hh"
#include <thread>
#include <exception>

#define BUFFER_SIZE 1024

class Server{
	public :
	 using Callback = std::function<void(long long, const Message &)>;
	private:
		int 				_socket;
		struct sockaddr_in  _serv_addr;
		long long _nextClientID = 1;
		std::unordered_map<long long, int> _clients;
		std::unordered_map<Message::Type, Callback> _actions;
		size_t _p_port;
		std::exception_ptr _exc;

		std::vector<pollfd> _pollFd;
		std::vector<std::string> _msg;

		std::mutex _mutex;
		std::thread	_loop;
		std::atomic<bool> _running;

		void _workerLoop();
	public:
		Server();
		Server(const Server&);
		Server& operator=(const Server&);
		~Server();

		void start(const size_t& p_port);
		void disconnect();

		void defineAction(const Message::Type& messageType, const std::function<void(long long clientID, const Message& msg)>& action);
		void sendTo(const Message& message, long long clientID);
		// void sendToArray(const Message& message, std::vector<long long> clientIDs);
		// void sendToAll(const Message& message);

		void update();
};
