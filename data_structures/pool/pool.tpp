/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 23:01:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 13:04:33 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pool.hh"


template<typename TType>
Pool<TType>::Pool() : _maxSize(0), _size(0){}

template<typename TType>
Pool<TType>::Pool(Pool &&other) : _pool(std::move(other._pool)), _poolSize(std::move(other._poolSize)),
		   _freeList(std::move(other._freeList)), _maxSize(other.maxSize()), _size(other.size()){
	other._pool.clear();
	other._poolSize.clear();
	other._freeList.clear();
	other._maxSize = 0;
	other._size = 0;
}

template<typename TType>
Pool<TType>& Pool<TType>::operator=(Pool && other){
	if (this != &other){
		_pool = std::move(other._pool);
		_poolSize = std::move(other._poolSize);
		_freeList = std::move(other._freeList);
		_maxSize = other._maxSize;
		_size = other._size;

		other._pool.clear();
		other._poolSize.clear();
		other._freeList.clear();
		other._maxSize = 0;
		other._size = 0;

	}
	return *this;
}

template<typename TType>
Pool<TType>::~Pool(){
	for (std::size_t it = 0; it < _pool.size(); it++)
		_alloc.deallocate(_pool[it], _poolSize[it]);
}

template<typename TType>
Pool<TType>::Object::~Object(){
	if (__obj){
		__freeList.push_back(__obj);
		if (__size > 0)
			__size--;
		std::allocator_traits<std::allocator<TType>>::destroy(__alloc, __obj);
	}
}

template<typename TType>
Pool<TType>::Object::Object(Object &&other) noexcept
: __obj(std::move(other.__obj)), __freeList(other.__freeList), __alloc(other.__alloc), __size(other.__size){
	other.__obj = nullptr;
}

template<typename TType>
Pool<TType>::Object&	Pool<TType>::Object::operator=(Object &&other) noexcept{
	if (this != &other){
		if (__obj){
			__freeList.push_back(__obj);
			if (__size > 0)
				__size--;
			std::allocator_traits<std::allocator<TType>>::destroy(__alloc, __obj);
		}
		__obj = std::move(other.__obj);
		__freeList = other.__freeList;
		__alloc = other.__alloc;
		__size = other.__size;

		other.__obj = nullptr;
	}
}

//--------------------------------------------
/// Allocates a certain number of TType objects
/// withing the Pool
//--------------------------------------------
template<typename TType>
void Pool<TType>::resize(const size_t &numberOfObjectStored) noexcept(false){
	if (numberOfObjectStored < 0 || numberOfObjectStored == _maxSize)
		return ;
	if (!_pool.empty()){
		if (numberOfObjectStored < _maxSize){
			_maxSize = numberOfObjectStored;
		} else if (numberOfObjectStored > _maxSize){
			std::size_t max(std::accumulate(_poolSize.begin(), _poolSize.end(), static_cast<size_t>(0)));
			if (numberOfObjectStored > max){
				std::size_t newSizePool = numberOfObjectStored - max;
				_pool.push_back(std::move(_alloc.allocate(newSizePool)));
				_poolSize.push_back(newSizePool);
				_freeList.reserve(max);
				for (std::size_t it = 0; it < newSizePool; it++)
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
/// Creates a Pool::Object containing a
/// pre-allocated object, using the constructor
/// with parameters as defined by TArgs definition.
//--------------------------------------------
template<typename TType> template<typename... TArgs>
Pool<TType>::Object Pool<TType>::acquire(TArgs &&...p_args) noexcept(false){
	if (_size >= _maxSize || _freeList.size() <= 0)
		throw std::out_of_range("out of range");

	Object node(_freeList.back(), _freeList, _alloc, _size);
	_freeList.pop_back();
	_traits.construct(_alloc, node.__obj, p_args...);
	_size++;
	return node;
}

template<typename TType>
Pool<TType>::Object::Object(TType *obj, std::vector<TType *> &freelist, std::allocator<TType> &alloc, std::size_t &size)
: __obj(obj), __freeList(freelist), __alloc(alloc), __size(size){}

template<typename TType>
TType*	Pool<TType>::Object::operator->(){return __obj;}

template<typename TType>
TType&	Pool<TType>::Object::operator*(){return *__obj;}

template<typename TType>
TType*	Pool<TType>::Object::get(){return __obj;}

template<typename TType>
std::size_t Pool<TType>::capacity() noexcept{return ((_size >= _maxSize) ? 0 : _maxSize - _size);}

template<typename TType>
std::size_t Pool<TType>::maxSize() noexcept{return _maxSize;}

template<typename TType>
std::size_t Pool<TType>::size() noexcept{return _size;}

template<typename TType>
bool Pool<TType>::empty() noexcept{return _pool.empty();}
