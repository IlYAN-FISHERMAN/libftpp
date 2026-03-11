/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:36:18 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/10 15:12:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "chronometer.hh"

lpp::chronometer::chronometer() : __name("chrono"), __time(std::chrono::seconds(0)){}

lpp::chronometer::chronometer(std::string name) : __name(name), __time(std::chrono::seconds(0)){}

lpp::chronometer::~chronometer(){}

void lpp::chronometer::start(){
	__time = std::chrono::steady_clock::now();
}

std::chrono::duration<double> lpp::chronometer::stop() const{
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double>{end - __time};
}

std::ostream& lpp::operator<<(std::ostream& os, const lpp::chronometer &chrono){
	os << chrono.__name << " took " << chrono.stop() << " seconds to run." << std::endl;
	return os;
}


lpp::unique_chrono::unique_chrono() : __chrono("unique_chrono"){
	__chrono.start();
}

lpp::unique_chrono::unique_chrono(std::string name) : __chrono(name){
		__chrono.start();
}

lpp::unique_chrono::~unique_chrono(){
	lpp::cout << __chrono << std::endl;
}
