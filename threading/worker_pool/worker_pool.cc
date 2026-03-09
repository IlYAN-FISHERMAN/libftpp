/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:56:32 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:09:25 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hh"

void lpp::worker_pool::_workLoop(){
	while(true){
		std::shared_ptr<IJobs> job;
		std::function<void()> funcJobs;
		bool isIJobs = false;
		{
			std::unique_lock lock(_mutex);
			_cv.wait(lock, [&]{return (_stop.load() || !_jobs.empty() || !_funcJobs.empty());});
			if (_stop.load() && _jobs.empty() && _funcJobs.empty())
				return ;

			try{
				if (!_jobs.empty()){
					job = _jobs.pop_front();
					isIJobs = true;
				}
				else if (!_funcJobs.empty()){
					funcJobs = _funcJobs.pop_front();
					isIJobs = false;
				}
			}catch(std::exception &e){
				lpp::cout.setPrefix("Thread job: ");
				lpp::cout << e.what() << std::endl;
				return;
			}
			_workerThread++;
		}

		if (isIJobs)
			job->execute();
		else
			funcJobs();
	
		// threadSafeCout << "thread: " << std::this_thread::get_id() << std::endl;
		// std::this_thread::sleep_for(std::chrono::seconds(2));

		{
			std::unique_lock lock(_mutex);
			_workerThread--;
			if (!_workerThread.load() && _jobs.empty() && _funcJobs.empty()){
				_cv.notify_all();
			}
		}
	}
}

lpp::worker_pool::worker_pool(size_t nbrOfThread) : _nbrOfThread(nbrOfThread), _stop(false), _workerThread(0){
	for (size_t it = 0; it < _nbrOfThread; it++)
		_threads.emplace_back(&worker_pool::_workLoop, this);
}

lpp::worker_pool::~worker_pool(){
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_stop.store(true);
		_cv.notify_all();
	}

	for (auto &t : _threads)
		t.join();
}

void lpp::worker_pool::wait(){
	std::unique_lock<std::mutex> lock(_mutex);
	_cv.wait(lock, [&]{ return ((_jobs.empty() && _funcJobs.empty()) && !_workerThread.load());});
}

void lpp::worker_pool::addJob(const std::function<void()> &jobToExecute){
	std::lock_guard<std::mutex> lock(_mutex);
	_funcJobs.push_back(jobToExecute);
	_cv.notify_one();
}

void lpp::worker_pool::addJob(std::shared_ptr<IJobs> jobToExecute){
	std::lock_guard<std::mutex> lock(_mutex);
	_jobs.push_back(jobToExecute);
	_cv.notify_one();
}

size_t lpp::worker_pool::running() const{return _workerThread.load();}
size_t lpp::worker_pool::size () const{return _jobs.size() + _funcJobs.size();}
