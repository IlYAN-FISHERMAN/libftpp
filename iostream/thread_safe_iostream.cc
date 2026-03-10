/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/10 15:14:45 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hh"

thread_local std::stringstream lpp::cout::_msg;
thread_local std::string lpp::cout::_prefix;

std::string lpp::cout::prefix() const{return _prefix;}

void lpp::cout::setPrefix(const std::string &str){_prefix = str;}

void lpp::cout::clear(){_prefix.clear();}

class lpp::cout& lpp::cout::operator<<(std::ostream& (*func)(std::ostream&)){
	std::lock_guard<std::mutex> lock(_mutex);

	std::cout << _prefix << _msg.str() << func;
	_msg.str("");
	_msg.clear();

	return *this;
}
