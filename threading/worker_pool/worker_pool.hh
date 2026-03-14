/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:50:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/14 15:03:26 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../iostream/thread_safe_iostream.hh"
#include "../thread_safe_queue/thread_safe_queue.hh"
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace lpp{
	class worker_pool : public lpp::NonCopyable, public lpp::NonMovable{
		public :
			class IJobs{
				public :
					virtual ~IJobs() = default;
					virtual void execute() = 0;
			};
		private:

			std::vector<std::thread>				_threads;
			thread_safe_queue<std::shared_ptr<IJobs>>	_jobs;
			thread_safe_queue<std::function<void()>>	_funcJobs;
			std::mutex								_mutex;
			std::condition_variable					_cv;
			std::size_t								_nbrOfThread;
			std::atomic<bool>						_stop;
			std::atomic<size_t>						_workerThread;

			void _workLoop();
		public:
			worker_pool(size_t);
			~worker_pool();

			void	addJob(const std::function<void()>&jobToExecute);
			void	addJob(std::shared_ptr<IJobs> jobToExecute);
			void	wait();

			size_t	running() const;
			size_t	size() const;
	};
}
