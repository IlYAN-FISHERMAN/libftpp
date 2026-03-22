/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.hh                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 10:26:56 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 10:24:29 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "mathematics/mathematics.hh"
#include <math.h>

namespace lpp{
	class perlin_noise_2D{
		private:
			lpp::random_2D_coordinate_generator _ran;
			float dot(float ix, float iy, float x, float y);
			float interpolate(float a0, float a1, float w) const;
		public:
			perlin_noise_2D();
			~perlin_noise_2D();

			float sample(float x, float y);
	};
}
