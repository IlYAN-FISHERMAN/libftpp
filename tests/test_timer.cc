/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_timer.cc                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:53:06 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 19:37:42 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testTimer(){
	lpp::timer time;
	lpp::unique_chrono chrono("timer");
	
	time.setInterval([]{
		lpp::cout << "i'm here every seconds" << std::endl;
	}, 1000);


	time.setInterval([]{
		lpp::cout << "i'm here every 5 seconds" << std::endl;
	}, 5000);


	time.setInterval([]{
		lpp::cout << "i'm here every 10 seconds" << std::endl;
	}, 10000);

	std::this_thread::sleep_for(std::chrono::seconds(20));
	return 0;
}
