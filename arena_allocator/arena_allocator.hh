/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_allocator.hh                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:30:13 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 16:44:14 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../threading/thread_safe_queue/thread_safe_queue.hh"
#include <functional>
#include <utility>

namespace lpp{
	class arena_allocator{
		private:
			lpp::thread_safe_queue<void *> _queue;
			lpp::thread_safe_queue<std::function<void()>> _deletor;

		public:
			arena_allocator(){}
			~arena_allocator(){
				while(!_deletor.empty())
					_deletor.pop_front()();

				while(!_queue.empty())
					::operator delete(_queue.pop_front());
			}

			template<typename TType, typename ...TArgs>
			TType* create(TArgs... args){
				auto *data = ::operator new (sizeof(TType));
				_queue.push_back(data);
				TType *obj = new (data) TType(std::forward<TArgs>(args)...);
				_deletor.push_back([obj]{
					obj->~TType();
				});
				return obj;
			}
			
	};
}
