/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:31:23 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 18:32:26 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <thread>
#include <functional>
#include "thread_safe_iostream.hh"

class Thread{
		private:
			Thread() = delete;
			Thread(const Thread&) = delete;
			Thread& operator=(const Thread&) = delete;

			std::string				_name;
			std::function<void()>	_func;
			std::thread				_thread;

			static inline void _runThread(const std::string &name, std::function<void()> functToExecute);
		public:
			~Thread();
			explicit Thread(const std::string& name, std::function<void()> functToExecute);

			void start();
			void stop();
};
