/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2026/01/26 13:30:49 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#pragma once

template <typename TType>
class Pool{
	public:
		struct Object{
			TType _obj;
			Object* next;
			Object* prev;
			Object(TType obj) : _obj(obj), next(NULL), prev(NULL){}
			Object(){}
			TType* operator->(){return &_obj;};
		};
	private:
		Object* _pool;
		size_t _len;
	public:
		Pool() : _pool(NULL), _len(0){}
		~Pool(){}

		//--------------------------------------------
		/// Allocates a certain number of TType objects
		/// withing the Pool
		//--------------------------------------------
		void resize(const size_t &numberOfObjectStored){
			if (numberOfObjectStored <= _len || numberOfObjectStored == 0)
				return ;
			for (auto it = _pool; it;){
				auto tmp = it;
				if (it->next)
					it = it->next;
				delete tmp;
			}
			_pool = new Object();
			size_t i = numberOfObjectStored - 1;
			for (auto it = _pool; i > 1; i--){
				it->next = new Object();
				it = it->next;
			}
		;}

		//--------------------------------------------
		/// Creates a Pool::Object containing a
		/// pre-allocated object, using the constructor
		/// with parameters as defined by TArgs definition.
		//--------------------------------------------
		// template<typename TArgs>
		// Pool::Object acquire(TArgs && p_args){
		// 	return Object(p_args);
		// }

		template<typename... TArgs>
		Pool::Object acquire(TArgs &&...p_args){
			return Object(p_args...);
		}
};
