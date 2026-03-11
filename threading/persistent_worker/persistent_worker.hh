/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hh                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:55:21 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 18:10:01 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../iostream/thread_safe_iostream.hh"
#include "../thread_safe_queue/thread_safe_queue.hh"
#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <set>

namespace lpp{
	class persistent_worker{
		public :
			class IJobs{
				public :
					virtual ~IJobs() = default;

					virtual void execute() = 0;
			};
		private :
			std::unordered_multimap<std::string, std::shared_ptr<IJobs>>	_jobs;
			std::unordered_multimap<std::string, std::function<void()>>		_funcJobs;
			std::set<std::string> _jobsName;
			std::thread				_thread;
			std::mutex				_mutex;
			std::condition_variable	_cv;
			std::atomic<bool>		_stop;
			std::atomic<bool>		_erase;

			persistent_worker(const persistent_worker &) = delete;
			persistent_worker& operator=(const persistent_worker&) = delete;

			void _workLoop();
		public :
			persistent_worker();
			~persistent_worker();

			void	addTask(const std::string& name, const std::function<void()>& jobToExecute);
			void	addTask(const std::string &name, std::shared_ptr<IJobs> jobToExecute);

			void 	removeTask(const std::string& name);
			bool	containe(const char *);
	};
}
