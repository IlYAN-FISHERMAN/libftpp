/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:02:36 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 18:06:30 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "data_buffer.hh"

template<typename T>
lpp::data_buffer& lpp::data_buffer::operator<<(T data) noexcept(false){
	std::vector<std::byte> byte(sizeof(T));
	memcpy(byte.data(), &data, sizeof(T));
	_bytes.push(byte);
	return *this;
}

template<typename T>
lpp::data_buffer& lpp::data_buffer::operator>>(T &data) noexcept(false){
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
