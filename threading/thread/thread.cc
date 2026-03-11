/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:34:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 19:20:20 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.hh"

lpp::thread::thread(){}

lpp::thread::~thread(){
	if (_thread.joinable())
		_thread.join();
}

lpp::thread::thread(const std::string& name, std::function<void()> functToExecute) : _name(name), _func(functToExecute){}

void lpp::thread::_runthread(const std::string &name, std::function<void()> functToExecute){
	lpp::cout.setPrefix(name);
	functToExecute();
}

void lpp::thread::start(){
	if (!_thread.joinable())
		_thread = std::thread(_runthread, _name, _func);
}

void lpp::thread::stop(){
	if (_thread.joinable())
		_thread.join();
}
