/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:50:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 16:58:24 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "thread_safe_iostream.hh"
#include "../thread_safe_queue/thread_safe_queue.hh"
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>


class WorkerPool{
	public :
		class IJobs{
			public :
				virtual ~IJobs() = default;
				virtual void execute() = 0;
		};
	private:

		std::vector<std::thread>				_threads;
		ThreadSafeQueue<std::shared_ptr<IJobs>>	_jobs;
		ThreadSafeQueue<std::function<void()>>	_funcJobs;
		std::mutex								_mutex;
		std::condition_variable					_cv;
		std::size_t								_nbrOfThread;
		std::atomic<bool>						_stop;

		WorkerPool(const WorkerPool &) = delete;
		WorkerPool& operator=(const WorkerPool&) = delete;

		void _workLoop();
	public:
		WorkerPool(size_t);
		~WorkerPool();

		void addJob(const std::function<void()>&jobToExecute);
		void addJob(std::shared_ptr<IJobs> jobToExecute);
		void wait();
};
