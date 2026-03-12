/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsability_chain.hh                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 12:02:38 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:19:28 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <memory>
#include "../../iostream/thread_safe_iostream.hh"

namespace lpp{
	class IResponsability_chain{
		public :
			virtual bool execute() = 0;
			virtual bool config() = 0;
			virtual ~IResponsability_chain() = default;
	};

	class responsability_chain{
		private :
			std::list<std::shared_ptr<IResponsability_chain>> _chain;
		public :
			responsability_chain();
			~responsability_chain();
			void setNext(std::shared_ptr<IResponsability_chain> next);
			void run();
	};
}
