/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 23:01:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/12 19:35:04 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pool.hh"

template<typename TType>
Pool<TType>::Object::~Object(){
	if (__obj){
		__freeList.push_back(__obj);
		__size--;
		std::allocator_traits<std::allocator<TType>>::destroy(__alloc, __obj);
	}
}

template<typename TType>
Pool<TType>::Object::Object(Object &&other) noexcept
: __obj(other.__obj), __freeList(other.__freeList), __alloc(other.__alloc), __size(other.__size){
	other.__obj = nullptr;
}

template<typename TType>
Pool<TType>::Object&	Pool<TType>::Object::operator=(Object &&other) noexcept{
	if (this != &other){
		if (__obj){
			__freeList.push_back(__obj);
			__size--;
			std::allocator_traits<std::allocator<TType>>::destroy(__alloc, __obj);
		}
		__obj = other.__obj;
		__freeList = other.__freeList;
		__alloc = other.__alloc;
		__size = other.__size;

		other.__obj = nullptr;
	}
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

