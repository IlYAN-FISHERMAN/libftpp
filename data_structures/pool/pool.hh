/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/14 15:48:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unordered_set>
#include <utility>
#include <numeric>
#include <vector>

#pragma once

template <typename TType>
class Pool{
	public:
		struct freeList{
			freeList* next;
		};

		class Object{
		protected :
			TType					*__obj;
			std::vector<TType *>	&__freeList;
			std::allocator<TType>	&__alloc;
			std::size_t				&__size;
			
			friend Pool;

			explicit Object(TType *, std::vector<TType *> &, std::allocator<TType> &, std::size_t &);

			Object() = delete;
			Object(const Object &) = delete;
			Object& operator=(const Object &) = delete;

		public :
			~Object();
			Object& operator=(Object &&) noexcept;
			Object(Object &&) noexcept;

			TType*	operator->();
			TType*	get();
			TType&	operator*();
		};
	private:
		std::vector<TType*>								_pool;
		std::vector<std::size_t>						_poolSize;
		std::allocator<TType>							_alloc;
		std::allocator_traits<std::allocator<TType>>	_traits;
		std::vector<TType *>							_freeList;
		std::size_t										_maxSize;
		std::size_t										_size;
	public:
		Pool() : _size(0){}

		~Pool(){
			for (std::size_t it = 0; it < _pool.size(); it++)
				_alloc.deallocate(_pool[it], _poolSize[it]);
		}

		std::size_t capacity() noexcept{return ((_size >= _maxSize) ? 0 : _maxSize - _size);}

		std::size_t maxSize() noexcept{return _maxSize;}

		std::size_t size() noexcept{return _size;}

		bool empty() noexcept{return _pool.empty();}

		//--------------------------------------------
		/// Allocates a certain number of TType objects
		/// withing the Pool
		//--------------------------------------------
		void resize(const size_t &numberOfObjectStored) noexcept(false){
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
		template<typename... TArgs>
		Pool::Object acquire(TArgs &&...p_args) noexcept(false){
			if (_size >= _maxSize || _freeList.size() <= 0)
				throw std::out_of_range("out of range");

			Object node(_freeList.back(), _freeList, _alloc, _size);
			_freeList.pop_back();
			_traits.construct(_alloc, node.__obj, p_args...);
			_size++;
			return node;
		}
};

#include "pool.tpp"
