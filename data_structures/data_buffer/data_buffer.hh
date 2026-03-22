/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:06:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 10:43:56 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <queue>
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
			data_buffer& operator<<(T data) noexcept(false);

			template<typename T>
			data_buffer& operator>>(T &data) noexcept(false);

			data_buffer& operator<<(const std::string&);

			data_buffer& operator>>(std::string&);
	};
	#include "data_buffer.tpp"
}
