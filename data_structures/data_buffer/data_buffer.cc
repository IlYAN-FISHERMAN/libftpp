/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:34:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 12:03:18 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "data_buffer.hh"

DataBuffer::DataBuffer(){}

DataBuffer::~DataBuffer(){}

DataBuffer& DataBuffer::operator=(const DataBuffer &other){
	if (this != &other){
		_bytes = other._bytes;
	}
	return *this;
}

DataBuffer::DataBuffer(const DataBuffer &other) : _bytes(other._bytes){}

DataBuffer& DataBuffer::operator<<(const std::string& s) {
	uint32_t len = s.size();
	*this << len;

	std::vector<std::byte> bytes(len);
	std::memcpy(bytes.data(), s.data(), len);
	_bytes.push(bytes);

	return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& s) {
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
