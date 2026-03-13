/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unique_memento.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:53:46 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 10:20:13 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "unique_memento.hh"

template<typename TType> template<typename ...TArgs>
lpp::unique_memento<TType>::unique_memento(TType &old, TArgs ...p_args) : _old(old), _save(old){
	_old = TType(p_args...);
}

template<typename TType>
lpp::unique_memento<TType>::~unique_memento(){
	_old = _save;
}
