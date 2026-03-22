/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:52 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 13:10:01 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include "design_patterns/design_patterns.hh"
#include "time/chronometer/chronometer.hh"
#include "network/logger/logger.hh"
#include "network/message/message.hh"

namespace lpp{
	class client : public IResponsability_chain, public lpp::NonCopyable{
		using Callback =  std::function<void(const message &)>;
		private:
			int	_socket;
			std::string _host;
			std::unordered_map<message::Type, Callback> _actions;
			bool _running;
			lpp::unique_chrono _chrono;
			lpp::logger _logger;

			bool config()	override;
			bool execute()	override;

			std::string received(std::string);
		public:
			client();
			~client();

			void connect(const std::string& address, const size_t& port);
			void disconnect();
			bool isRunning();

			void defineAction(const message::Type& messageType, const std::function<void(const message& msg)>& action);
			std::string send(const message& message);
			std::string send(std::string&, bool = true);
			std::string send(const char*, size_t);
	};
}
