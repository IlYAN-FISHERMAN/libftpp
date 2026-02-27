/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:21:49 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../iostream/thread_safe_iostream.hh"
#include "../network.hh"
#include "../threading/threading.hh"

#define BUFFER_SIZE 1024

class Server{
	public :
	 using Callback = std::function<void(long long&, const Message&)>;
	private:
		int 				_socket;
		struct sockaddr_in  _serv_addr;

		std::mutex _mutex;

		long long _nextClientID = 1;
		std::unordered_map<long long, int> _clients;
		std::unordered_map<Message::Type, Callback> _actions;
		PersistentWorker	_loop;
	public:
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
// class Server {
// public:
//
//     void start(uint16_t port) {
//         _thread = std::thread([this, port]{ run(port); });
//     }
//
//     void sendTo(const Message& msg, long long clientID) {
//         std::lock_guard<std::mutex> lock(_mutex);
//         int sock = _clients[clientID];
//         std::string s = msg.str();
//         ::send(sock, s.c_str(), s.size(), 0);
//     }
//
// private:
//
//     void run(uint16_t port) {
//         int serverSock = socket(AF_INET, SOCK_STREAM, 0);
//         sockaddr_in addr{};
//         addr.sin_family = AF_INET;
//         addr.sin_addr.s_addr = INADDR_ANY;
//         addr.sin_port = htons(port);
//         bind(serverSock, (sockaddr*)&addr, sizeof(addr));
//         listen(serverSock, 5);
//
//         while (true) {
//             int clientSock = accept(serverSock, nullptr, nullptr);
//             if (clientSock < 0) continue;
//             long long clientID = _nextClientID++;
//             {
//                 std::lock_guard<std::mutex> lock(_mutex);
//                 _clients[clientID] = clientSock;
//             }
//
//             std::thread([this, clientSock, clientID]{
//                 char buffer[1024];
//                 while (true) {
//                     ssize_t n = recv(clientSock, buffer, sizeof(buffer), 0);
//                     if (n <= 0) break;
//                     // Minimal deserialization : juste le type + contenu
//                     Message msg(static_cast<Message::Type>(buffer[0]));
//                     for (ssize_t i = 1; i < n; ++i)
//                         msg << buffer[i];
//
//                     std::lock_guard<std::mutex> lock(_mutex);
//                     if (_actions.count(msg.type()))
//                         _actions[msg.type()](clientID, msg);
//                 }
//                 close(clientSock);
//                 std::lock_guard<std::mutex> lock(_mutex);
//                 _clients.erase(clientID);
//             }).detach();
//         }
//     }
// };
