/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 23:01:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 11:31:00 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pool.hh"


template<typename TType>
lpp::pool<TType>::pool() : _maxSize(0), _size(0), _alive(std::make_shared<poolAlive>()){}

template<typename TType>
lpp::pool<TType>::~pool(){
	for (std::size_t it = 0; it < _pool.size(); it++)
		_alloc.deallocate(_pool[it], _poolSize[it]);
	if (_alive)
		_alive->alive = false;
}


template<typename TType>
lpp::pool<TType>::Object::Object(TType *obj, pool *pool) : __obj(obj), __pool(pool), __alive(pool->_alive){}

template<typename TType>
lpp::pool<TType>::Object::Object(Object &&other) noexcept
: __obj(std::move(other.__obj)), __pool(other.__pool), __alive(other.__alive){
	other.__obj = nullptr;
}

template<typename TType>
lpp::pool<TType>::Object::~Object(){

	auto lock = __alive.lock();
	if (!lock)
		return;
	if ((lock || lock->alive) && __obj){
		if (__pool){
			__pool->_freeList.push_back(__obj);
			if (__pool->_size > 0)
				__pool->_size--;
		}
		std::allocator_traits<std::allocator<TType>>::destroy(__pool->_alloc, __obj);
	}
}

template<typename TType>
typename lpp::pool<TType>::Object&	lpp::pool<TType>::Object::operator=(Object &&other) noexcept{
	if (this != &other){
	auto lock = __alive.lock();
		if ((lock || lock->alive) && __obj){
			__pool->_freeList.push_back(__obj);
			if (__pool->_size > 0)
				__pool->_size--;
			std::allocator_traits<std::allocator<TType>>::destroy(__pool->_alloc, __obj);
		}
		__obj = std::move(other.__obj);
		__pool = other.__pool;

		other.__obj = nullptr;
	}
}

//--------------------------------------------
/// Allocates a certain number of TType objects
/// withing the pool
//--------------------------------------------
template<typename TType>
void lpp::pool<TType>::resize(const size_t &numberOfObjectStored) noexcept(false){
	if (numberOfObjectStored < 0 || numberOfObjectStored == _maxSize)
		return ;
	if (!_pool.empty()){
		if (numberOfObjectStored < _maxSize){
			_maxSize = numberOfObjectStored;
		} else if (numberOfObjectStored > _maxSize){
			std::size_t max(std::accumulate(_poolSize.begin(), _poolSize.end(), static_cast<size_t>(0)));
			if (numberOfObjectStored > max){
				std::size_t newSizepool = numberOfObjectStored - max;
				_pool.push_back(std::move(_alloc.allocate(newSizepool)));
				_poolSize.push_back(newSizepool);
				_freeList.reserve(max);
				for (std::size_t it = 0; it < newSizepool; it++)
					_freeList.push_back((_pool.back() + it));
			}
			_maxSize = numberOfObjectStored;
		}
	} else if (_pool.empty()){
		_pool.push_back(_alloc.allocate(numberOfObjectStored));
		_poolSize.push_back(numberOfObjectStored);
		_maxSize = numberOfObjectStored;
		_freeList.reserve(numberOfObjectStored);
		for (std::size_t it = 0; it < _maxSize; it++)
			_freeList.push_back((_pool.back() + it));
	}
}

//--------------------------------------------
/// Creates a pool::Object containing a
/// pre-allocated object, using the constructor
/// with parameters as defined by TArgs definition.
//--------------------------------------------
template<typename TType> template<typename... TArgs>
typename lpp::pool<TType>::Object lpp::pool<TType>::acquire(TArgs &&...p_args) noexcept(false){
	if (_size >= _maxSize || _freeList.size() <= 0)
		throw std::out_of_range("out of range");

	Object node(_freeList.back(), this);
	_freeList.pop_back();
	_traits.construct(_alloc, node.__obj, p_args...);
	_size++;
	return (node);
}

template<typename TType>
TType*	lpp::pool<TType>::Object::operator->(){
	auto lock = __alive.lock();
	if (!lock || !__alive.lock().get()->alive)
		throw std::logic_error("pool was destroyed");
	if (!__obj)
		throw std::range_error("Object is empty");

	return __obj;
}

template<typename TType>
TType&	lpp::pool<TType>::Object::operator*(){return *__obj;}

template<typename TType>
TType*	lpp::pool<TType>::Object::get(){return __obj;}

template<typename TType>
std::size_t lpp::pool<TType>::capacity() noexcept{return ((_size >= _maxSize) ? 0 : _maxSize - _size);}

template<typename TType>
std::size_t lpp::pool<TType>::maxSize() noexcept{return _maxSize;}

template<typename TType>
std::size_t lpp::pool<TType>::size() noexcept{return _size;}

template<typename TType>
bool lpp::pool<TType>::empty() noexcept{return _pool.empty();}
