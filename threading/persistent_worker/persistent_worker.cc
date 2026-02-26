/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:39:33 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/26 19:35:08 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_worker.hh"

void PersistentWorker::_workLoop(){
	while(true){
		std::vector<std::shared_ptr<IJobs>>	jobs;
		std::vector<std::function<void()>>	funcJobs;

		{
			std::unique_lock lock(_mutex);
			_cv.wait(lock, [&]{return (_stop.load() || !_jobs.empty() || !_funcJobs.empty());});

			if (_stop.load())
				return ;

			for (auto name = _jobsName.begin(); name != _jobsName.end(); name++){
				auto jobRange = _jobs.equal_range(*name);
				for (;jobRange.first != jobRange.second; jobRange.first++)
					jobs.push_back(jobRange.first->second);

				auto funcRange = _funcJobs.equal_range(*name);
				for (;funcRange.first != funcRange.second; funcRange.first++)
					funcJobs.push_back(funcRange.first->second);
			}
		}
		for (auto job : jobs)
			job->execute();
		for (auto func : funcJobs)
			func();
	}
}

PersistentWorker::PersistentWorker(){
	_stop.store(false);
	_thread = std::thread(&PersistentWorker::_workLoop, this);
}

PersistentWorker::~PersistentWorker(){
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_stop.store(true);
		_cv.notify_one();
	}
	_thread.join();
}

void PersistentWorker::addTask(const std::string &name, const std::function<void()> &jobToExecute){
	std::lock_guard<std::mutex> lock(_mutex);
	_funcJobs.insert({name, jobToExecute});
	_jobsName.insert(name);
	_cv.notify_one();
}

void PersistentWorker::addTask(const std::string &name, std::shared_ptr<IJobs> jobToExecute){
	std::lock_guard<std::mutex> lock(_mutex);
	_jobs.insert({name, jobToExecute});
	_jobsName.insert(name);
	_cv.notify_one();
}

void PersistentWorker::removeTask(const std::string& name){
	std::unique_lock<std::mutex> lock(_mutex);
	{
		auto range = _jobs.equal_range(name);
		if (range.first != range.second)
			_jobs.erase(range.first, range.second);
	}
	{
		auto range = _funcJobs.equal_range(name);
		if (range.first != range.second)
			_funcJobs.erase(range.first, range.second);
	}
	_jobsName.erase(name);
}

// size_t PersistentWorker::size() const{return _workerThread.load();}
