/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cc                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 11:13:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:52:20 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "random_2D_coordinate_generator.hh"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>

lpp::random_2D_coordinate_generator::random_2D_coordinate_generator(){
		srand(time(NULL));
		__seed = rand() % 1000;
}

lpp::random_2D_coordinate_generator::~random_2D_coordinate_generator(){}

long long lpp::random_2D_coordinate_generator::operator()(const long long& x, const long long& y, long mod){
	long seed = __seed;
	std::string tmp;
	if (mod == 0)
		mod = __seed;
	long long randomValue = static_cast<long long>(std::pow(x + seed, 2) + std::pow(y + seed, 2));

	for (;seed > 0; seed--){
		tmp = std::to_string(static_cast<long long>(std::pow(randomValue, 2)));
		if (tmp.size() >= 6 && std::stoll(tmp.substr((tmp.size() / 2) - 2, 4)) != 0)
			randomValue = std::stoll(tmp.substr((tmp.size() / 2) - 2, 4));
		else
			randomValue = std::stoll(tmp);
	}
	return randomValue % mod;
}

long long lpp::random_2D_coordinate_generator::seed(){return __seed;}
