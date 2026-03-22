/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hh                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 10:23:05 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <mutex>
#include <sstream>

namespace lpp{
	class cout{
		private :
			static inline std::mutex _mutex;
			static thread_local std::stringstream _msg;
			static thread_local std::string _prefix;

			cout& operator=(const cout&) = delete;
			cout& operator=(const cout&&) = delete;

		public :
			std::string prefix() const;

			template<typename T>
			cout& operator<<(T data){
				std::lock_guard<std::mutex> lock(_mutex);
				_msg << data;
				return *this;
			}

			cout& operator<<(std::ostream& (*func)(std::ostream&));

			void setPrefix(const std::string &str);
			void clear();

			template<typename T>
			void prompt(const std::string& question, T& dest, bool lockCout = false){

				if (lockCout){
					std::unique_lock<std::mutex> lock(_mutex);
					std::cout << question << std::flush;
					std::getline(std::cin, dest);
				} else{
					*this << question << std::flush;
					std::getline(std::cin, dest);
				}
			}
	};

	static thread_local class cout cout;
}
