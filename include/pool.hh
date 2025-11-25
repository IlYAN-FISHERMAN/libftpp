/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:24:05 by ilyanar           #+#    #+#             */
/*   Updated: 2025/09/26 19:14:34 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#pragma once

template <typename TType>
class Pool{
	public:
		class Object{
			private:
				TType* _obj;
				Object* next;
				Object* prev;
			public :
			TType* operator->(){return _obj;};
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
			if (numberOfObjectStored == _len)
				return ;
			Object* begin = _pool;
			for (size_t i = 0; i < _len; i++)
				;
			for (size_t i = 0; i < numberOfObjectStored; i++)
			_pool[i] = Object();
		;}

		//--------------------------------------------
		/// Creates a Pool::Object containing a
		/// pre-allocated object, using the constructor
		/// with parameters as defined by TArgs definition.
		//--------------------------------------------
		template<typename TArgs>
		Pool::Object acquire(TArgs && p_args){
			(void)p_args;
			return Pool::Object();
		}

		template<typename... TArgs>
		Pool::Object acquire(TArgs &&...p_args){
		return acquire(p_args...);
	}
};
