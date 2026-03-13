/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_allocator.tpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:53:11 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 18:06:54 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "arena_allocator.hh"

template<typename TType, typename ...TArgs>
TType* lpp::arena_allocator::create(TArgs... args){
	auto *data = ::operator new (sizeof(TType));
	_queue.push_back(data);
	TType *obj = new (data) TType(std::forward<TArgs>(args)...);
	_deletor.push_back([obj]{
		obj->~TType();
	});
	return obj;
}
