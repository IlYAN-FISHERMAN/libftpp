/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.hh                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:09:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 10:47:23 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "threading/thread/thread.hh"

namespace lpp{
	class timer{
		private:
			std::vector<std::thread> _worker;
			std::atomic<bool>		_running;
		public:
			timer();
			~timer();

			void setInterval(std::function<void()> func, size_t delay);

			void stop();
	};
}
