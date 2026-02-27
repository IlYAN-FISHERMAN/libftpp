/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:52 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:22:25 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../network.hh"
#include <unordered_map>
#include <string>

class Client{
	using Callback =  std::function<void(const Message &)>;
	private:
		int	_socket;
		std::unordered_map<Message::Type, Callback> _actions;

	public:
		Client();

		void connect(const std::string& address, const size_t& port);
		void disconnect();

		void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);
		void send(const Message& message);
		void update();
};
