/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/17 12:31:59 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unordered_set>
#include <utility>
#include <numeric>
#include <vector>
#include <memory>
#include <set>
#include "../../design_patterns/non_copyable/non_copyable.hh"
#include "../../utils/color.hh"

#pragma once

namespace lpp{
	template <typename TType>
	class pool : public lpp::NonCopyable, public lpp::NonMovable{
		public:
			struct poolAlive{
				bool alive;
				poolAlive() : alive(true){}
			};

			class Object : public lpp::NonCopyable{
				protected :
					TType							*__obj;
					pool							*__pool;
					std::weak_ptr<poolAlive>		__alive;
					
					friend pool;

					explicit Object(TType *, pool *);

					Object() = delete;
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
			std::shared_ptr<poolAlive>						_alive;

		public:
			pool();
			~pool();

			std::size_t capacity() noexcept;

			std::size_t maxSize() noexcept;

			std::size_t size() noexcept;

			bool empty() noexcept;

			//--------------------------------------------
			/// Allocates a certain number of TType objects
			/// withing the pool
			//--------------------------------------------
			void resize(const size_t &numberOfObjectStored) noexcept(false);

			//--------------------------------------------
			/// Creates a pool::Object containing a
			/// pre-allocated object, using the constructor
			/// with parameters as defined by TArgs definition.
			//--------------------------------------------
			template<typename... TArgs>
			pool::Object acquire(TArgs &&...p_args) noexcept(false);
	};
}

#include "pool.tpp"
