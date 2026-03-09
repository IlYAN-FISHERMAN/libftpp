/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:31:23 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:55:36 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <thread>
#include <functional>
#include "../../iostream/thread_safe_iostream.hh"

namespace lpp{
	class thread{
			private:
				thread() = delete;
				thread(const thread&) = delete;
				thread& operator=(const thread&) = delete;

				std::string				_name;
				std::function<void()>	_func;
				std::thread				_thread;

				static inline void _runthread(const std::string &name, std::function<void()> functToExecute);
			public:
				~thread();
				explicit thread(const std::string& name, std::function<void()> functToExecute);

				void start();
				void stop();
	};
}
