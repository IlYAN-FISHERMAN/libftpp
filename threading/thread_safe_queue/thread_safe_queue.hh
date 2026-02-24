/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hh                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:44:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 16:31:44 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <utility>

template<typename TType>
class ThreadSafeQueue{
	private :
		std::deque<TType> _queue;
		static inline std::mutex _mutex;
	public :
		ThreadSafeQueue(){}
		~ThreadSafeQueue(){}
		ThreadSafeQueue(const ThreadSafeQueue &other){
			std::scoped_lock<std::mutex> lock(_mutex, other._mutex);
			_queue = other._queue;
		}
		ThreadSafeQueue& operator=(const ThreadSafeQueue &other){
			if (this != &other){
				std::scoped_lock<std::mutex> lock(_mutex, other._mutex);
				_queue = other._queue;
			}
			return *this;
		}
	
		void push_back(TType&& newElement) noexcept{
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_back(std::forward<TType>(newElement));
		};

		void push_front(TType&& newElement) noexcept{
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_front(std::forward<TType>(newElement));
		}

		void push_back(const TType& newElement) noexcept{
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_back(newElement);
		};

		void push_front(const TType& newElement) noexcept{
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_front(newElement);
		}

		TType pop_back() noexcept(false){
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.size() <= 0)
				throw std::runtime_error("Empty queue");
			auto pop = std::move(_queue.back());
			_queue.pop_back();
			return pop;
		};

		TType pop_front() noexcept(false){
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.size() <= 0)
				throw std::runtime_error("Empty queue");
			auto pop = std::move(_queue.front());
			_queue.pop_front();
			return pop;
		};

		bool empty(){ 
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.empty();
		}
};
