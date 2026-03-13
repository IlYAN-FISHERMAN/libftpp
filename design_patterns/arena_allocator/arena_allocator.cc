/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_allocator.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:51:15 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 17:51:04 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "arena_allocator.hh"
#include "../../iostream/thread_safe_iostream.hh"

lpp::arena_allocator::arena_allocator(){}

lpp::arena_allocator::arena_allocator(lpp::arena_allocator &&other) : _queue(std::move(other._queue)), _deletor(std::move(other._deletor)){}

lpp::arena_allocator& lpp::arena_allocator::operator=(arena_allocator &&other){

	if (this != &other){
		if (!_queue.empty())
			_queue.clear();
		if (!_deletor.empty())
			_deletor.clear();
		_queue = std::move(other._queue);
		_deletor = std::move(other._deletor);
		other._queue.clear();
		other._deletor.clear();
	}

	return *this;
}

lpp::arena_allocator::~arena_allocator(){
	while(!_deletor.empty())
		_deletor.pop_front()();

	while(!_queue.empty())
		::operator delete(_queue.pop_front());
}

