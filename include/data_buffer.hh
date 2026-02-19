/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:06:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 10:44:27 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <queue>
#include <bit>
#include <cstdint>
#include <cstddef>
#include <cstring>

class DataBuffer{
	private:
		std::queue<std::vector<std::byte>> _bytes;
	public:
		template<typename T>
		DataBuffer& operator<<(T data) noexcept(false){
			std::vector<std::byte> byte(sizeof(T));
			memcpy(byte.data(), &data, sizeof(T));
			_bytes.push(byte);
			return *this;
		}

		template<typename T>
		DataBuffer& operator>>(T &data) noexcept(false){
			if (_bytes.empty())
				throw std::logic_error("no more object to deserialize");
			std::vector<std::byte> byte(_bytes.front());
			_bytes.pop();
			T tmp;
			memcpy(&tmp, byte.data(), sizeof(T));
			data = tmp;
			return *this;
		}

		DataBuffer& operator<<(std::string &data) noexcept(false){
			std::vector<std::byte> byte(sizeof(std::string));
			memcpy(byte.data(), data.c_str(), sizeof(std::string));
			_bytes.push(byte);
			return *this;
		}

		DataBuffer& operator>>(std::string &data) noexcept(false){
			if (_bytes.empty())
				throw std::logic_error("no more object to deserialize");
			std::vector<std::byte> byte(_bytes.front());
			_bytes.pop();
			char tmp[100000];
			memcpy(&tmp, byte.data(), sizeof(std::string));
			data = tmp;
			return *this;
		}
};
