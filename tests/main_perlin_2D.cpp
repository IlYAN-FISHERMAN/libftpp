/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_perlin_2D.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 10:26:02 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/15 16:06:26 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testPerlin_noise_2D(){
	lpp::perlin_noise_2D perlin;
	lpp::unique_chrono chrono("perlin");

    const int gridSize = 100;
    char visualChars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@', '\0'};

	lpp::cout << "Sampling 2D Perlin noise over a " << gridSize << "x" << gridSize << " grid:" << std::endl << std::endl;

    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            float sample = perlin.sample(x * 0.21f, y * 0.10f); // Adjust these factors as needed
            sample = (sample + 1) / 2; // Map from [-1, 1] to [0, 1]
            int charIndex = std::round(sample * 9); // Map from [0, 1] to [0, 9]

            lpp::cout << visualChars[charIndex] << " ";
        }
        lpp::cout << std::endl;
    }

    return 0;
}

