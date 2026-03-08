/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hh                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:39:28 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/08 10:23:43 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Random2DCoordinateGenerator{
	private:
		long __seed;
	public:
		Random2DCoordinateGenerator();
		~Random2DCoordinateGenerator();

		long long seed();
		long long operator()(const long long& x, const long long& y, long mod = 0);
};
