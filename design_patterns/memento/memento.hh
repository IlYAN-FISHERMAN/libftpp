/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:55:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/20 19:21:24 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <any>
#include <queue>

class Memento{
		public :
			class Snapshot{
				private :
					std::queue<std::any> _state;
				public :
					Snapshot();
					~Snapshot();
					Snapshot(const Snapshot&);
					Snapshot& operator=(const Snapshot&);

					template<typename T>
					Memento::Snapshot& operator<<(T &data){
						_state.push(data);
						return *this;
					};

					template<typename T>
					Memento::Snapshot& operator>>(T &data){
						try{
							data = std::any_cast<T>(_state.front());
							_state.pop();
						}catch(std::bad_any_cast &e){
							throw e;
						}
						return *this;
					};
			};
	protected :
		virtual void _saveToSnapshot(Memento::Snapshot &) const = 0;
		virtual void _loadFromSnapshot(Memento::Snapshot &) = 0;
		Snapshot	_state;

		Memento();
		virtual ~Memento();

	public :

		void load(const Memento::Snapshot& state);
		Snapshot save();
};

