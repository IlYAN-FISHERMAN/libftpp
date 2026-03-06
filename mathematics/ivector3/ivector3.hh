/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:18:03 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/06 18:34:29 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <algorithm>
#include <exception>
#include <math.h>

template<typename TType>
class IVector3{
	public:
		TType x;
		TType y;
		TType z;

		IVector3() : x(0), y(0), z(0){}
		IVector3(TType x, TType y, TType z) : x(x), y(y), z(z){}
		~IVector3(){}


		float length(){return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));}

		IVector3<float> normalize(){
			float len = length();
			return IVector3<float>(x / len, y / len, z / len);
		}

		float dot(const IVector3 other){return static_cast<float>((x * other.x) + (y * other.y) + (z * other.z));}

		IVector3 cross(const IVector3 &other){
			return IVector3(
				(y * other.z) - (z * other.y),
				(z * other.x) - (x * other.z),
				(x * other.y) - (y * other.x));
		}

		IVector3 operator+(const IVector3 &other){return IVector3(x + other.x, y + other.y, z + other.z);}
		IVector3 operator-(const IVector3 &other){return IVector3(x - other.x, y - other.y, z - other.z);}
		IVector3 operator*(const IVector3 &other){return IVector3(x * other.x, y * other.y, z * other.z);}

		IVector3 operator/(const IVector3<TType> &other){
			if (other.x == 0|| other.y == 0 || other.z == 0)
				throw std::overflow_error("Divide by zero exception");
			return IVector3(x / other.x, y / other.y, z / other.z);
		}

		bool operator==(const IVector3<TType> &other){return (x == other.x && y == other.y && z == other.z);}
		bool operator!=(const IVector3<TType> &other){return (x != other.x && y != other.y && z != other.z);}
		bool operator<(const IVector3<TType> &other){return (x < other.x && y < other.y && z < other.z);}
		bool operator>(const IVector3<TType> &other){return (x > other.x && y > other.y && z > other.z);}
};
