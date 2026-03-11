/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer.hh                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:36:18 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 18:55:35 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <chrono>
#include <iostream>
#include "../../iostream/thread_safe_iostream.hh"

namespace lpp{
	class chronometer{
		private :
			std::string __name;
			std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1LL, 1000000000LL>>> __time;
		public :
			chronometer();
			chronometer(std::string);
			~chronometer();

			void start();
			std::chrono::duration<double> stop() const;
			friend std::ostream& operator<<(std::ostream& os, const lpp::chronometer &chrono);
	};

	class unique_chrono{
		private:
			lpp::chronometer __chrono;
		public:
			unique_chrono();
			unique_chrono(std::string);
			~unique_chrono();
	};
	std::ostream& operator<<(std::ostream& os, const lpp::chronometer &chrono);
}
