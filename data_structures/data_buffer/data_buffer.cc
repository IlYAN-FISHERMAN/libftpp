/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:34:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:47:45 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "data_buffer.hh"

lpp::data_buffer::data_buffer(){}

lpp::data_buffer::~data_buffer(){}

lpp::data_buffer& lpp::data_buffer::operator=(const data_buffer &other){
	if (this != &other){
		_bytes = other._bytes;
	}
	return *this;
}

lpp::data_buffer::data_buffer(const data_buffer &other) : _bytes(other._bytes){}

lpp::data_buffer& lpp::data_buffer::operator<<(const std::string& s) {
	uint32_t len = s.size();
	*this << len;

	std::vector<std::byte> bytes(len);
	std::memcpy(bytes.data(), s.data(), len);
	_bytes.push(bytes);

	return *this;
}

lpp::data_buffer& lpp::data_buffer::operator>>(std::string& s) {
	uint32_t len;
	*this >> len;
	if (_bytes.empty())
		throw std::logic_error("no more object to deserialize");

	auto& buf = _bytes.front();
	if (buf.size() != len)
		throw std::logic_error("invalid string length");

	s.assign(reinterpret_cast<const char*>(buf.data()), len);
	_bytes.pop();
	return *this;
}
