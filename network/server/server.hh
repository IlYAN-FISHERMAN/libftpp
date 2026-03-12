/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:45:59 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../client/client.hh"
#include "../../design_patterns/responsability_chain/responsability_chain.hh"
#include "../../design_patterns/non_copyable/non_copyable.hh"
#include <thread>
#include <exception>

#define BUFFER_SIZE 1024

namespace lpp{
	class server : public lpp::IResponsability_chain, public lpp::NonCopyable{
		public :
		 using Callback = std::function<void(long long, const message &)>;
		private:
			int 				_socket;
			struct sockaddr_in  _serv_addr;
			std::unordered_map<long long, int> _clients;
			std::unordered_map<message::Type, Callback> _actions;
			size_t _p_port;
			std::exception_ptr _exc;

			std::vector<pollfd> _pollFd;
			std::vector<std::string> _msg;

			std::mutex _mutex;
			std::thread	_loop;
			std::atomic<bool> _running;

			void _workerLoop();

			bool config()	override;
			bool execute()	override;

		public:
			server();
			~server();

			void start(const size_t& p_port);
			void disconnect();

			void defineAction(const message::Type& messageType, const std::function<void(long long clientID, const message& msg)>& action);
			void sendTo(const message& message, long long clientID);
	};
}
