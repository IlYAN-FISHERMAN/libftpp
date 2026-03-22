/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unique_memento.hh                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:54:04 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/22 10:21:53 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "design_patterns/non_copyable/non_copyable.hh"

namespace lpp{
	template <typename TType>
	class unique_memento : public lpp::NonCopyable, public lpp::NonMovable{
		private:
			TType &_old;
			TType _save;
		public:
			template<typename ...TArgs>
			unique_memento(TType&, TArgs...);
			~unique_memento();
	};
	#include "unique_memento.tpp"
}
