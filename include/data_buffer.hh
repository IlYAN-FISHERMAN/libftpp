/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:06:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/18 16:55:31 by ilyanar          ###   LAUSANNE.ch       */
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
			memcpy(byte.data(), &data, sizeof(std::string));
			_bytes.push(byte);
			return *this;
		}

		DataBuffer& operator>>(std::string &data) noexcept(false){
			if (_bytes.empty())
				throw std::logic_error("no more object to deserialize");
			std::vector<std::byte> byte(_bytes.front());
			_bytes.pop();
			std::string tmp;
			tmp.resize(sizeof(byte.size()));
			memmove(tmp.data(), byte.data(), sizeof(std::string));
			data = tmp;
			return *this;
		}
};
