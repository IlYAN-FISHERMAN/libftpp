/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hh                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 19:51:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:49:00 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <functional>

namespace lpp{
	template<typename TEvent, typename ...TType>
	class observer{
		private:
			std::unordered_multimap<TEvent, std::function<void(TType&...)>>	_events;

		public:
			observer(){};
			
			~observer(){};
		
			observer(const observer &other) : _events(other._events){};

			observer& operator=(const observer &other){
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
}
