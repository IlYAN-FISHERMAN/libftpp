/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cc                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 11:13:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/08 10:24:04 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "random_2D_coordinate_generator.hh"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(){
		srand(time(NULL));
		__seed = rand() % 1000;
}

Random2DCoordinateGenerator::~Random2DCoordinateGenerator(){}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y, long mod){
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

long long Random2DCoordinateGenerator::seed(){return __seed;}
