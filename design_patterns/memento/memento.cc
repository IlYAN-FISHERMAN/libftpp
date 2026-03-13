/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:55:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 17:44:32 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "memento.hh"

lpp::memento::memento(){}

lpp::memento::~memento(){}

lpp::memento::memento(const memento &other) : _state(other._state){}

lpp::memento& lpp::memento::operator=(const memento &other){
	if (this != &other)
		_state = other._state;

	return *this;
}

void lpp::memento::load(const memento::Snapshot& state){
	if (!state._state.empty()){
		_state = state;
		_loadFromSnapshot(_state);
	}
}

lpp::memento::Snapshot lpp::memento::save(){
	_saveToSnapshot(_state);
	return _state;
}

lpp::memento::Snapshot::Snapshot(){}

lpp::memento::Snapshot::~Snapshot(){}

lpp::memento::Snapshot::Snapshot(const Snapshot &other) : _state(other._state){}

lpp::memento::Snapshot& lpp::memento::Snapshot::operator=(const Snapshot &other){
	if (this != &other){
		_state = other._state;
	}

	return *this;
}
