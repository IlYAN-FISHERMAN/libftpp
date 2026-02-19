/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 12:22:23 by ilyanar          ###   LAUSANNE.ch       */
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

		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;

	public:
		Pool();

		~Pool();
	
		Pool(Pool&&);

		Pool& operator=(Pool&&);


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
