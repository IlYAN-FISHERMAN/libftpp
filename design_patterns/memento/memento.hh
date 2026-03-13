/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:55:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 17:44:02 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <any>
#include <queue>

namespace lpp{
	class memento{
			public :
				class Snapshot{
					friend memento;
					private :
						std::queue<std::any> _state;
					public :
						Snapshot();
						~Snapshot();
						Snapshot(const Snapshot&);
						Snapshot& operator=(const Snapshot&);

						template<typename T>
						memento::Snapshot& operator<<(T &data){
							_state.push(data);
							return *this;
						};

						template<typename T>
						memento::Snapshot& operator>>(T &data){
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
			virtual void _saveToSnapshot(memento::Snapshot &) const = 0;
			virtual void _loadFromSnapshot(memento::Snapshot &) = 0;
			Snapshot	_state;

			memento();
			memento(const memento&);
			memento& operator=(const memento&);
			virtual ~memento();

		public :

			void load(const memento::Snapshot& state);
			Snapshot save();
	};

}
