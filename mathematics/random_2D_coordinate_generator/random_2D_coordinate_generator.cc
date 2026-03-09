/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cc                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 11:13:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 16:30:09 by ilyanar          ###   LAUSANNE.ch       */
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

lpp::ivector2<float> lpp::random_2D_coordinate_generator::operator()(const float ix, const float iy){
	const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; 
    unsigned a = ix, b = iy;

    a *= 3284157443;
    b ^= a << s | a >> (w - s);
    b *= 1911520717;
 
    a ^= b << s | b >> (w - s);
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1));

	return ivector2(sin(random), cos(random));
}

long long lpp::random_2D_coordinate_generator::seed(){return __seed;}

