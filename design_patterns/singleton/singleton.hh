/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hh                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:17:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:49:09 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <memory>

namespace lpp{
	template<typename TType>
	class singleton{
		private:
			singleton(const singleton&) = delete;
			singleton& operator=(const singleton&) = delete;
			singleton() = delete;
			~singleton() = delete;

			static inline std::shared_ptr<TType> _instance = nullptr;
		public:

			static TType* instance() noexcept(false){
				if (!_instance)
					throw std::logic_error("Instance not yet created");
				return _instance.get();
			};
			
			template<typename ... TArgs>
			static void instantiate(TArgs&& ...p_args) noexcept(false){
				if (_instance)
					throw std::logic_error("Instance already created");
				_instance = std::make_shared<TType>(p_args...);
			};
	};
}
