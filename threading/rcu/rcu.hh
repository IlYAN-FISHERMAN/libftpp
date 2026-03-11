/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rcu.hh                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:33:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 10:03:55 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <atomic>

namespace lpp{
	template<typename TArgs>
	class rcu {
		private:
			std::atomic<TArgs> __data;

		public:
			rcu();
			~rcu();
			TArgs load();
			TArgs store();
	};
}
