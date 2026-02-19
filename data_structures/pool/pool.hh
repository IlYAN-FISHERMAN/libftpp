/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 19:53:09 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unordered_set>
#include <utility>
#include <numeric>
#include <vector>
#include <memory>
#include <set>

#pragma once

template <typename TType>
class Pool{
	public:
		struct PoolAlive{
			bool alive;
			PoolAlive() : alive(true){}
		};

		class Object{
			protected :
				TType							*__obj;
				Pool							*__pool;
				std::weak_ptr<PoolAlive>		__alive;
				
				friend Pool;

				explicit Object(TType *, Pool *);

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
				bool	ok();
		};
	private:
		std::vector<TType*>								_pool;
		std::vector<std::size_t>						_poolSize;
		std::allocator<TType>							_alloc;
		std::allocator_traits<std::allocator<TType>>	_traits;
		std::vector<TType *>							_freeList;
		std::size_t										_maxSize;
		std::size_t										_size;
		std::shared_ptr<PoolAlive>						_alive;

		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;
		Pool(Pool&&) = delete;
		Pool& operator=(Pool&&) = delete;

	public:
		Pool();
		~Pool();

		std::size_t capacity() noexcept;

		std::size_t maxSize() noexcept;

		std::size_t size() noexcept;

		bool empty() noexcept;

		//--------------------------------------------
		/// Allocates a certain number of TType objects
		/// withing the Pool
		//--------------------------------------------
		void resize(const size_t &numberOfObjectStored) noexcept(false);

		//--------------------------------------------
		/// Creates a Pool::Object containing a
		/// pre-allocated object, using the constructor
		/// with parameters as defined by TArgs definition.
		//--------------------------------------------
		template<typename... TArgs>
		Pool::Object acquire(TArgs &&...p_args) noexcept(false);
};

#include "pool.tpp"
