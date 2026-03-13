/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:18:03 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 10:01:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <algorithm>
#include <exception>
#include <math.h>

namespace lpp{
	template<typename TType>
	class ivector3{
		public:
			TType x;
			TType y;
			TType z;

			ivector3() : x(0), y(0), z(0){}
			ivector3(TType x, TType y, TType z) : x(x), y(y), z(z){}
			ivector3(const ivector3& other) : x(other.x), y(other.y), z(other.z){}
			ivector3& operator=(const ivector3& other){
				if (this != &other){
					x = other.x;
					y = other.y;
					z = other.z;
				}

				return *this;
			}

			~ivector3(){}


			float length(){return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));}

			ivector3<float> normalize(){
				float len = length();
				return ivector3<float>(x / len, y / len, z / len);
			}

			float dot(const ivector3 other){return static_cast<float>((x * other.x) + (y * other.y) + (z * other.z));}

			ivector3 cross(const ivector3 &other){
				return ivector3(
					(y * other.z) - (z * other.y),
					(z * other.x) - (x * other.z),
					(x * other.y) - (y * other.x));
			}

			ivector3 operator+(const ivector3 &other){return ivector3(x + other.x, y + other.y, z + other.z);}
			ivector3 operator-(const ivector3 &other){return ivector3(x - other.x, y - other.y, z - other.z);}
			ivector3 operator*(const ivector3 &other){return ivector3(x * other.x, y * other.y, z * other.z);}

			ivector3 operator/(const ivector3<TType> &other){
				if (other.x == 0|| other.y == 0 || other.z == 0)
					throw std::overflow_error("Divide by zero exception");
				return ivector3(x / other.x, y / other.y, z / other.z);
			}

			friend bool operator==(const ivector3<TType> &first, const ivector3 &second){return (first.x == second.x && first.y == second.y && first.z == second.z);}
			friend bool operator!=(const ivector3<TType> &first, const ivector3 &second){return (first.x != second.x && first.y != second.y && first.z != second.z);}
			bool operator<(const ivector3<TType> &other){return (x < other.x && y < other.y && z < other.z);}
			bool operator>(const ivector3<TType> &other){return (x > other.x && y > other.y && z > other.z);}

			friend std::ostream& operator<<(std::ostream &os, const ivector3 &v){
				os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;;

				return os;
			}
	};
}
