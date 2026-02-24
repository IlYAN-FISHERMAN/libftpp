/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:56:32 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 17:04:54 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hh"

void WorkerPool::_workLoop(){
	while(true){
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [&]{return (_stop.load() || !_jobs.empty() || !_funcJobs.empty());});
		if (_stop.load() && _jobs.empty() && _funcJobs.empty())
			break;
		if (!_jobs.empty()){
			try{
				std::move(_jobs.pop_front())->execute();
			}catch(std::exception &e){
				threadSafeCout.setPrefix("Thread IJbos: ");
				threadSafeCout << e.what() << std::endl;
			}
		}
		else if (!_funcJobs.empty()){
			try{
				std::move(_funcJobs.pop_front())();
			}catch(std::exception &e){
				threadSafeCout.setPrefix("Thread IJbos: ");
				threadSafeCout << e.what() << std::endl;
			}
		}
	}
}

WorkerPool::WorkerPool(size_t nbrOfThread) : _nbrOfThread(nbrOfThread), _stop(false){
	for (size_t it = 0; it < _nbrOfThread; it++)
		_threads.emplace_back(&WorkerPool::_workLoop, this);
}

WorkerPool::~WorkerPool(){
	_stop.store(true);
	_cv.notify_all();

	for (auto &t : _threads)
		t.join();
}

void WorkerPool::wait(){

	_stop.store(true);
	_cv.notify_all();

	for (auto &t : _threads)
		t.join();

	_stop.store(false);

	_threads.clear();
	for (size_t it = 0; it < _nbrOfThread; it++)
		_threads.emplace_back(&WorkerPool::_workLoop, this);
}

void WorkerPool::addJob(const std::function<void()>&jobToExecute){
	_funcJobs.push_back(jobToExecute);
	_cv.notify_one();
}

void WorkerPool::addJob(std::shared_ptr<IJobs> jobToExecute){
	_jobs.push_back(jobToExecute);
	_cv.notify_one();
}
