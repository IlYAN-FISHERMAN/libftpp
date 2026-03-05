/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:17:16 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/05 19:31:39 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <iterator>
#include <iostream>
#include <functional>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <functional>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <cstddef>
#include <poll.h>
#include <csignal>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <pool/pool.hh>
#include "../iostream/thread_safe_iostream.hh"

class Message{
	public:
		using Type = int;
	private:
		Type				_type;
		mutable std::stringstream	_value;
	public:
		Message(int type);
		Message();

		Type type() const;

		template<typename TType>
		Message& operator<<(TType other){
			_value << other;
			return *this;
		}

		template<typename TType>
		friend const Message& operator>>(const Message& msg, TType &other){
			// if (!msg._value.eof())
			msg._value >> other;
			return msg;
		}

	std::string str() const { return _value.str(); }
	bool eof() const{return _value.eof();}
    std::stringstream& valueStream() { return _value; }

};
