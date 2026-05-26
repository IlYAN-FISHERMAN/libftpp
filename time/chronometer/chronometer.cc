/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:36:18 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/26 18:25:24 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "chronometer.hh"
using namespace std::chrono_literals;

lpp::chronometer::chronometer() : _name("chrono"), _time(0s){}

lpp::chronometer::chronometer(std::string name) : _name(name), _time(0s){}

lpp::chronometer::~chronometer(){}

void lpp::chronometer::start(){
	_time = std::chrono::steady_clock::now();
}

std::chrono::duration<double> lpp::chronometer::stop() const{
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double>{end - _time};
}

std::ostream& lpp::operator<<(std::ostream& os, const lpp::chronometer &chrono){
	os << chrono._name << " took " << chrono.stop() << " seconds to run." << std::endl;
	return os;
}


lpp::unique_chrono::unique_chrono() : _chrono("unique_chrono"){
	_chrono.start();
}

lpp::unique_chrono::unique_chrono(std::string name) : _chrono(name){
		_chrono.start();
}

lpp::unique_chrono::~unique_chrono(){
	lpp::cout << lpp::logger::getLog(lpp::INFO) << _chrono;
}
