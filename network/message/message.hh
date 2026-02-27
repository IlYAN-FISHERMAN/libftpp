/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:17:16 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:01:28 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>
#include <arpa/inet.h>

class Message{
	public:
		enum Type : int{
			INT = 1,
			SIZE_T = 2,
			DOUBLE = 3,
		};
	private:
		Type _type;
		std::stringstream _value;
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
			std::stringstream tmp;
			tmp << msg._value.rdbuf();
			tmp >> other;

			return msg;
		}

	std::string str() const { return _value.str(); }
    std::stringstream& valueStream() { return _value; }

};
