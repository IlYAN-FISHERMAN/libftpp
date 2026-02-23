/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:34:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 18:33:39 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.hh"

Thread::~Thread(){
	if (_thread.joinable())
		_thread.join();
}

Thread::Thread(const std::string& name, std::function<void()> functToExecute) : _name(name), _func(functToExecute){}

void Thread::_runThread(const std::string &name, std::function<void()> functToExecute){
	threadSafeCout.setPrefix(name);
	functToExecute();
}

void Thread::start(){
	if (!_thread.joinable())
		_thread = std::thread(_runThread, _name, _func);
}

void Thread::stop(){
	if (_thread.joinable())
		_thread.join();
}
