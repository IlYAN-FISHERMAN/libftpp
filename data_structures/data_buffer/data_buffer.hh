/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:06:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:47:43 by ilyanar          ###   LAUSANNE.ch       */
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

namespace lpp{
	class data_buffer{
		private:
			std::queue<std::vector<std::byte>> _bytes;
		public:
			data_buffer();
			~data_buffer();
			data_buffer(const data_buffer&);
			data_buffer& operator=(const data_buffer&);

			template<typename T>
			data_buffer& operator<<(T data) noexcept(false){
				std::vector<std::byte> byte(sizeof(T));
				memcpy(byte.data(), &data, sizeof(T));
				_bytes.push(byte);
				return *this;
			}

			template<typename T>
			data_buffer& operator>>(T &data) noexcept(false){
				static_assert(std::is_trivially_copyable_v<T>,
					  "data_buffer can only deserialize trivially copyable types");
				if (_bytes.empty())
					throw std::logic_error("no more object to deserialize");
				if (_bytes.front().size() != sizeof(T))
					throw std::logic_error("can't deserialize");
				std::vector<std::byte> byte(_bytes.front());
				_bytes.pop();
				memcpy(&data, byte.data(), sizeof(T));
				return *this;
			}

			data_buffer& operator<<(const std::string&);

			data_buffer& operator>>(std::string&);
	};
}
