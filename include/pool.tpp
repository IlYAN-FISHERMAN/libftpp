/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 23:01:53 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/11 18:39:04 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pool.hh"

template<typename TType>
Pool<TType>::Object::~Object(){
	__freeList.push_back(__obj);
	std::allocator_traits<std::allocator<TType>>::destroy(__alloc, __obj);
}

template<typename TType>
Pool<TType>::Object::Object(TType *obj, std::vector<TType *> &freelist, std::allocator<TType> &alloc)
: __obj(obj), __freeList(freelist), __alloc(alloc){}

template<typename TType>
Pool<TType>::Object::Object()
: __obj(nullptr), __freeList(nullptr){}

template<typename TType>
TType*	Pool<TType>::Object::operator->(){return __obj;}

template<typename TType>
TType&	Pool<TType>::Object::operator*(){return *__obj;}

template<typename TType>
TType*	Pool<TType>::Object::get(){return __obj;}

