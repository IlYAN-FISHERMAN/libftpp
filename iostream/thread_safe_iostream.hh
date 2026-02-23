/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hh                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 14:48:25 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

class threadSafeCout {
	private :
		static inline std::mutex _mutex;
		static thread_local std::stringstream _msg;
		static thread_local std::string _prefix;

		threadSafeCout& operator=(const threadSafeCout&) = delete;
		threadSafeCout& operator=(const threadSafeCout&&) = delete;
	public :

		std::string prefix() const;

		template<typename T>
		threadSafeCout& operator<<(T &data){
			std::lock_guard<std::mutex> lock(_mutex);
			_msg << data;
			return *this;
		}


		threadSafeCout& operator<<(std::ostream& (*func)(std::ostream&)){
			std::lock_guard<std::mutex> lock(_mutex);

			std::cout << _prefix << _msg.str() << func;
			_msg.str("");
			_msg.clear();

			return *this;
		}

		void setPrefix(const std::string &str);

		template<typename T>
		void prompt(const std::string& question, T& dest){
			std::lock_guard<std::mutex> lock(_mutex);

			std::cout << _prefix << question;
			std::cout.flush();
			std::cin >> dest;
		}
};

static thread_local class threadSafeCout threadSafeCout;
