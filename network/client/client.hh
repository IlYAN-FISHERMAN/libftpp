/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:52 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:54:11 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../message/message.hh"
#include <mutex>
#include <thread>
#include <arpa/inet.h>

namespace lpp{
	class client{
		using Callback =  std::function<void(const message &)>;
		private:
			int	_socket;
			std::string _host;
			std::unordered_map<message::Type, Callback> _actions;

		public:
			client();
			~client();

			void connect(const std::string& address, const size_t& port);
			void disconnect();

			void defineAction(const message::Type& messageType, const std::function<void(const message& msg)>& action);
			void send(const message& message);
			void send(std::string&);
	};
}
