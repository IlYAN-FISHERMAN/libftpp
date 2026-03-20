/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:52 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/20 11:37:09 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../message/message.hh"
#include <mutex>
#include <thread>
#include "../../design_patterns/responsability_chain/responsability_chain.hh"
#include "../../design_patterns/non_copyable/non_copyable.hh"
#include <arpa/inet.h>

namespace lpp{
	class client : public IResponsability_chain, public lpp::NonCopyable{
		using Callback =  std::function<void(const message &)>;
		private:
			int	_socket;
			std::string _host;
			std::unordered_map<message::Type, Callback> _actions;
			bool _running;

			bool config()	override;
			bool execute()	override;
		public:
			client();
			~client();

			void connect(const std::string& address, const size_t& port);
			void disconnect();
			bool isRunning();

			void defineAction(const message::Type& messageType, const std::function<void(const message& msg)>& action);
			void send(const message& message);
			std::string send(std::string&, bool = true);
			std::string send(const char*, size_t);
	};
}
