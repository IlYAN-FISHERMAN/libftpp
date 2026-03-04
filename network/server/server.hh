/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/04 11:06:19 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <../threading/persistent_worker/persistent_worker.hh>
#include "../client/client.hh"

#define BUFFER_SIZE 1024

class Server : public PersistentWorker::IJobs{
	public :
	 using Callback = std::function<void(long long&, const Message&)>;
	private:
		int 				_socket;
		struct sockaddr_in  _serv_addr;
		long long _nextClientID = 1;
		std::unordered_map<long long, int> _clients;
		std::unordered_map<Message::Type, Callback> _actions;
		std::vector<pollfd> _pollFd;
		size_t _p_port;

		std::mutex _mutex;

		PersistentWorker	_loop;
		std::vector<std::string> _msg;
	public:
		virtual void execute() override;
		Server();
		Server(const Server&);
		Server& operator=(const Server&);
		~Server();

		void start(const size_t& p_port);
		void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);

		void sendTo(const Message& message, long long clientID);
		// void sendToArray(const Message& message, std::vector<long long> clientIDs);
		// void sendToAll(const Message& message);

		void update();
};
