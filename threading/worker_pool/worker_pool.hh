/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:50:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 19:02:45 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "thread_safe_iostream.hh"
#include <functional>
#include <thread>

class WorkerPool{
	private:
		class IJobs{
			private :
			public :
		};

		std::unordered_map<std::string, IJobs> _jobs;
	public:
		WorkerPool(size_t);
		void addJob(const std::function<void()>&jobToExecute);
};
