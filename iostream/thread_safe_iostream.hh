/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hh                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 14:15:56 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

class threadSafeCout {
	private :
		static inline std::mutex _mutex;

		static std::stringstream& msg();

	public :
		threadSafeCout(){}

		static std::string& prefix();

		template<typename T>
		threadSafeCout& operator<<(T &data){
			msg() << data;
			return *this;
		}


		threadSafeCout& operator<<(std::ostream& (*func)(std::ostream&)){
			std::lock_guard<std::mutex> lock(_mutex);

			std::cout << prefix() << msg().str() << func;
			msg().str("");
			msg().clear();

			return *this;
		}

		void setPrefix(const std::string &str){
			prefix() = str;
		};

		template<typename T>
		void prompt(const std::string& question, T& dest);
};

static threadSafeCout	threadSafeCout;
