/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_allocator.cc                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:51:15 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:53:09 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "arena_allocator.hh"

lpp::arena_allocator::arena_allocator(){}

lpp::arena_allocator::~arena_allocator(){
	while(!_deletor.empty())
		_deletor.pop_front()();

	while(!_queue.empty())
		::operator delete(_queue.pop_front());
}

