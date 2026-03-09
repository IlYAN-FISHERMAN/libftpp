/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:18:03 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:50:26 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <algorithm>
#include <exception>
#include <math.h>

namespace lpp{
	template<typename TType>
	class ivector2{
		public:
			TType x;
			TType y;

			ivector2() : x(0), y(0){}
			ivector2(TType x, TType y) : x(x), y(y){}
			~ivector2(){}


			float length(){return std::sqrt(std::pow(x, 2) + std::pow(y, 2));}

			ivector2<float> normalize(){
				float len = length();
				return ivector2<float>(x / len, y / len);
			}

			float dot(const ivector2 other){return static_cast<float>((x * other.x) + (y * other.y));}

			double cross(const ivector2 &other){return (x * other.y) - (y * other.x);};

			ivector2 cross(){return ivector2(-y, x);};

			ivector2 operator+(const ivector2 &other){return ivector2(x + other.x, y + other.y);}
			ivector2 operator-(const ivector2 &other){return ivector2(x - other.x, y - other.y);}
			ivector2 operator*(const ivector2 &other){return ivector2(x * other.x, y * other.y);}

			ivector2 operator/(const ivector2<TType> &other){
				if (other.x == 0|| other.y == 0)
					throw std::overflow_error("Divide by zero exception");
				return ivector2(x / other.x, y / other.y);
			}

			bool operator==(const ivector2<TType> &other){return (x == other.x && y == other.y);}
			bool operator!=(const ivector2<TType> &other){return (x != other.x && y != other.y);}
			bool operator<(const ivector2<TType> &other){return (x < other.x && y < other.y);}
			bool operator>(const ivector2<TType> &other){return (x > other.x && y > other.y);}
	};
}
