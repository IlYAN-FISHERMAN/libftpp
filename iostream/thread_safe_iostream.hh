/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hh                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/21 21:19:10 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <mutex>

class threadSafeCout{
	private :
		std::string	_prefix;
		mutable std::mutex	_mutex;
	public :

		// template<typename T>
		// friend const class threadSafeCout& operator<<(const class threadSafeCout &os, T &data);

		template<typename T>
		threadSafeCout& operator<<(const T data){
			std::lock_guard<std::mutex> lock(_mutex);
			std::cout << data;
			return *this;
		}

		threadSafeCout& operator<<(std::ostream& (*func)(std::ostream&)){
			std::lock_guard<std::mutex> lock(_mutex);
			std::cout << func;
			return *this;
		}

		void setPrefix(const std::string&);

		// template<typename T>
		// void prompt(const std::string& question, T& dest);
};

static threadSafeCout threadSafeCout;
