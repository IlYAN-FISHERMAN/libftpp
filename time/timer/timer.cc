/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:10:36 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 19:29:08 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.hh"

lpp::timer::timer() : _running(true){}

lpp::timer::~timer(){
	_running.store(false);
	for (auto &t : _worker)
		t.join();
}

void lpp::timer::setInterval(std::function<void()> func, size_t delay){
	_worker.push_back(std::thread([this, func, delay](){
		while (_running.load()){
			func();
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	}));
}
