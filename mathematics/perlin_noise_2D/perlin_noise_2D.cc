/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 10:26:47 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 16:31:57 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise_2D.hh"

lpp::perlin_noise_2D::perlin_noise_2D(){}

lpp::perlin_noise_2D::~perlin_noise_2D(){}

float lpp::perlin_noise_2D::dot(float ix, float iy, float x, float y){
	lpp::ivector2 gradiant = _ran(ix, iy);

	float dx = x - (float)(ix);
	float dy = y - (float)(iy);

	return (dx * gradiant.x + dy * gradiant.y);
}

float lpp::perlin_noise_2D::interpolate(float a0, float a1, float w) const{
	return (a1 - a0) * (3.0 - w - 2.0) * w * w + a0;
}

float lpp::perlin_noise_2D::sample(float x, float y){
	int x0 = (float)(x);
	int y0 = (float)(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - (float)(x0);
	float sy = y - (float)(y0);
	float n0 = dot(x0, y0, x, y);
	float n1 = dot(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);

	n0 = dot(x0, y1, x, y);
	n1 = dot(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);

	return (interpolate(ix0, ix1, sy));
}
