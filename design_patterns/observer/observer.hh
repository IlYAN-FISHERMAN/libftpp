/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 19:51:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 17:31:53 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>

template<typename TEvent, typename ...TType>
class Observer{
	private:
		std::unordered_multimap<TEvent, std::function<void(TType&...)>>	_events;

	public:
		Observer(){};
		
		~Observer(){};
	
		Observer(const Observer &other) : _events(other._events){};

		Observer& operator=(const Observer &other){
			if (this != &other){
				_events = other._events;
			}
			return *this;
		};

		void subscribe(const TEvent& event, const std::function<void(TType...)> lambda){
			_events.insert({event, lambda});
		}

		void notify(const TEvent& event, TType ...data){
			auto elements = _events.equal_range(event);
			for (auto func = elements.first; func != elements.second; func++)
				func->second(data...);
		}
};
