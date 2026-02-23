/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hh                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:17:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 12:55:19 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <memory>

template<typename TType>
class Singleton{
	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton() = delete;
		~Singleton() = delete;

		static inline std::shared_ptr<TType> _instance = nullptr;
	public:

		static TType* instance() noexcept(false){
			if (!_instance)
				throw std::logic_error("Instance not yet created");
			return _instance.get();
		};
		
		template<typename ... TArgs>
		static void instantiate(TArgs&& ...p_args) noexcept(false){
			if (_instance) // Remove this ?
				throw std::logic_error("Instance already created");
			_instance = std::make_shared<TType>(p_args...);
		};
};
