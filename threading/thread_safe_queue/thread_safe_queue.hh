/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hh                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:44:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/26 14:10:26 by ilyanar          ###   LAUSANNE.ch       */
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
		class iterator{
			
		};
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

		template<typename ...TArgs>
		void emplace_front(TArgs &&...p_args){
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.emplace_front(p_args...);
		}

		template<typename ...TArgs>
		void emplace_back(TArgs &&...p_args){
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.emplace_back(p_args...);
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

		bool empty() const{
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.empty();
		}

		size_t size() const{
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.size();
		}

		size_t max_size() const{
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.max_size();
		}

		void resize(size_t n){
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.resize(n);
		}

		void clear(){
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.clear();
		}
};
