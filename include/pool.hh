/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/12 14:48:29 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

#pragma once

template <typename TType>
class Pool{
	public:
		class Object{
		protected :
			TType *__obj;
			std::vector<TType *> &__freeList;
			std::allocator<TType> &__alloc;
			
			friend Pool;

			explicit Object(TType *obj, std::vector<TType *> &freelist, std::allocator<TType> &alloc);
			Object();
		public :
			~Object();

			TType*	operator->();
			TType*	get();
			TType&	operator*();
		};
	private:
		TType *_pool;
		std::allocator<TType> _alloc;
		std::allocator_traits<std::allocator<TType>> _traits;
		std::vector<TType *> _freeList;
		std::size_t			_maxSize;
	public:
		Pool() : _pool(nullptr), _maxSize(0){}
		~Pool(){
			if (_pool)
				_alloc.deallocate(_pool, _maxSize * sizeof(TType));
		}

		//--------------------------------------------
		/// Allocates a certain number of TType objects
		/// withing the Pool
		//--------------------------------------------
		void resize(const size_t &numberOfObjectStored){
			if (numberOfObjectStored < 0 || numberOfObjectStored == _maxSize)
				return ;
			if (_pool){
				for (std::size_t it = 0; it < _maxSize; it++)
					_traits.destroy(_alloc, _pool + it);
				_alloc.deallocate(_pool, _maxSize * sizeof(TType));
				_pool = nullptr;
				_freeList.clear();
				_maxSize = 0;
			}
			_pool = _alloc.allocate(numberOfObjectStored * sizeof(TType));
			_maxSize = numberOfObjectStored;
			for (std::size_t it = 0; it < _maxSize; it++){
				_freeList.push_back((_pool + it));
			}
			// just for testing
			// std::cout << "created a pool of " << numberOfObjectStored * sizeof(TType) << " bytes" << std::endl;
		}

		//--------------------------------------------
		/// Creates a Pool::Object containing a
		/// pre-allocated object, using the constructor
		/// with parameters as defined by TArgs definition.
		//--------------------------------------------
		template<typename... TArgs>
		Pool::Object acquire(TArgs &&...p_args) noexcept(false){
			if (_freeList.size() <= 0)
				throw std::out_of_range("out of range");

			Object node(_freeList.back(), _freeList, _alloc);
			_freeList.pop_back();
			_traits.construct(_alloc, node.__obj, p_args...);
			return node;
		}
};

#include "pool.tpp"
