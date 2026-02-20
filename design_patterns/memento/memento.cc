/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:55:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/20 21:28:50 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "memento.hh"

Memento::Memento(){}

Memento::~Memento(){}

Memento::Memento(const Memento &other) : _state(other._state){}

Memento& Memento::operator=(const Memento &other){
	if (this != &other)
		_state = other._state;

	return *this;
}

void Memento::load(const Memento::Snapshot& state){
	_state = state;
	_loadFromSnapshot(_state);
}

Memento::Snapshot Memento::save(){
	_saveToSnapshot(_state);
	return _state;
}

Memento::Snapshot::Snapshot(){}

Memento::Snapshot::~Snapshot(){}

Memento::Snapshot::Snapshot(const Snapshot &other) : _state(other._state){
}

Memento::Snapshot& Memento::Snapshot::operator=(const Snapshot &other){
	if (this != &other)
		_state = other._state;

	return *this;
}
