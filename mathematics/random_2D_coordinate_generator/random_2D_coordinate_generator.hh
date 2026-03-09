/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.hh                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:39:28 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:51:32 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace lpp{
	class random_2D_coordinate_generator{
		private:
			long __seed;
		public:
			random_2D_coordinate_generator();
			~random_2D_coordinate_generator();

			long long seed();
			long long operator()(const long long& x, const long long& y, long mod = 0);
	};
}
