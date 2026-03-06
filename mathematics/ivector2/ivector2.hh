/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:18:03 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/06 18:12:48 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <algorithm>
#include <exception>
#include <math.h>

template<typename TType>
class IVector2{
	public:
		TType x;
		TType y;

		IVector2() : x(0), y(0){}
		IVector2(TType x, TType y) : x(x), y(y){}
		~IVector2(){}


		float length(){return std::sqrt(std::pow(x, 2) + std::pow(y, 2));}

		IVector2<float> normalize(){
			float len = length();
			return IVector2<float>(x / len, y / len);
		}

		float dot(const IVector2 other){return static_cast<float>((x * other.x) + (y * other.y));}

		double cross(const IVector2 &other){return (x * other.y) - (y * other.x);};

		IVector2 cross(){return IVector2(-y, x);};

		IVector2 operator+(const IVector2 &other){return IVector2(x + other.x, y + other.y);}
		IVector2 operator-(const IVector2 &other){return IVector2(x - other.x, y - other.y);}
		IVector2 operator*(const IVector2 &other){return IVector2(x * other.x, y * other.y);}

		IVector2 operator/(const IVector2<TType> &other){
			if (other.x == 0|| other.y == 0)
				throw std::overflow_error("Divide by zero exception");
			return IVector2(x / other.x, y / other.y);
		}

		bool operator==(const IVector2<TType> &other){return (x == other.x && y == other.y);}
		bool operator!=(const IVector2<TType> &other){return (x != other.x && y != other.y);}
		bool operator<(const IVector2<TType> &other){return (x < other.x && y < other.y);}
		bool operator>(const IVector2<TType> &other){return (x > other.x && y > other.y);}
};
