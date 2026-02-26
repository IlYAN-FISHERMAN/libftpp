/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cc                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:39:33 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/26 20:29:37 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_worker.hh"

void PersistentWorker::_workLoop(){
	while(true){
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [&]{
			return ((!_stop.load() && !_erase.load() && !_jobsName.empty()) || (_stop.load()));
		});

		if (_stop.load())
			return ;

		for (auto name = _jobsName.begin(); name != _jobsName.end(); name++){
			auto jobRange = _jobs.equal_range(*name);
			for (;jobRange.first != jobRange.second; jobRange.first++)
				jobRange.first->second->execute();

			auto funcRange = _funcJobs.equal_range(*name);
			for (;funcRange.first != funcRange.second; funcRange.first++)
				funcRange.first->second();
		}
	}
}

PersistentWorker::PersistentWorker(){
	_stop.store(false);
	_erase.store(false);
	_thread = std::thread(&PersistentWorker::_workLoop, this);
}

PersistentWorker::~PersistentWorker(){
	_stop.store(true);
	_thread.join();
}

void PersistentWorker::addTask(const std::string &name, const std::function<void()> &jobToExecute){
	std::lock_guard<std::mutex> lock(_mutex);
	_funcJobs.insert({name, jobToExecute});
	_jobsName.insert(name);
	_cv.notify_one();
}

void PersistentWorker::addTask(const std::string &name, std::shared_ptr<IJobs> jobToExecute){
	_erase.store(true);
	std::lock_guard<std::mutex> lock(_mutex);
	_jobs.insert({name, std::move(jobToExecute)});
	_jobsName.insert(name);
	_erase.store(false);
	_cv.notify_one();
}

void PersistentWorker::removeTask(const std::string& name){
	_erase.store(true);
	std::unique_lock<std::mutex> lock(_mutex);
	auto range = _jobs.equal_range(name);
	_jobs.erase(range.first, range.second);
	auto range2 = _funcJobs.equal_range(name);
	_funcJobs.erase(range2.first, range2.second);
	_jobsName.erase(name);
	_erase.store(false);
	_cv.notify_one();
}

// size_t PersistentWorker::size() const{return _workerThread.load();}
