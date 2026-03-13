/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_allocator.hh                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:30:13 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 17:50:12 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../threading/thread_safe_queue/thread_safe_queue.hh"
#include "../non_copyable/non_copyable.hh"
#include <functional>
#include <utility>

namespace lpp{
	class arena_allocator : public lpp::NonCopyable{
		private:
			lpp::thread_safe_queue<void *> _queue;
			lpp::thread_safe_queue<std::function<void()>> _deletor;

		public:
			arena_allocator();
			~arena_allocator();
			arena_allocator(arena_allocator&&);
			arena_allocator& operator=(arena_allocator&&);

			template<typename TType, typename ...TArgs>
			TType* create(TArgs... args);
	};
	#include "arena_allocator.tpp"
}
