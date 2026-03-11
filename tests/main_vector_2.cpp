/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_vector_2.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:12:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/11 11:38:47 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testIVector2() {
	lpp::unique_chrono chrono("ivector2");
	lpp::ivector2<int> vec1(3, 4);
	lpp::ivector2<int> vec2(1, 2);

    lpp::cout << "Vec1 : " << vec1.x << " / " << vec1.y << std::endl;
    lpp::cout << "Vec2 : " << vec2.x << " / " << vec2.y << std::endl;


    // Test operator overloads
    auto vecAdd = vec1 + vec2;
    lpp::cout << "vec1 + vec2 = (" << vecAdd.x << ", " << vecAdd.y << ")" << std::endl; 
    // Expected: vec1 + vec2 = (4, 6)

    auto vecSub = vec1 - vec2;
    lpp::cout << "vec1 - vec2 = (" << vecSub.x << ", " << vecSub.y << ")" << std::endl; 
    // Expected: vec1 - vec2 = (2, 2)

    auto vecMul = vec1 * vec2;
    lpp::cout << "vec1 * vec2 = (" << vecMul.x << ", " << vecMul.y << ")" << std::endl; 
    // Expected: vec1 * vec2 = (3, 8)

    auto vecDiv = vec1 / vec2;
    lpp::cout << "vec1 / vec2 = (" << vecDiv.x << ", " << vecDiv.y << ")" << std::endl; 
    // Expected: vec1 / vec2 = (3, 2)

    bool isEqual = vec1 == vec2;
    lpp::cout << "vec1 == vec2: " << std::boolalpha << isEqual << "" << std::endl; 
    // Expected: vec1 == vec2: false

    bool isNotEqual = vec1 != vec2;
    lpp::cout << "vec1 != vec2: " << std::boolalpha << isNotEqual << "" << std::endl; 
    // Expected: vec1 != vec2: true

    // Test additional methods
    float len = vec1.length();
    lpp::cout << "Length of vec1: " << len << "" << std::endl; 
    // Expected: Length of vec1: 5 (or sqrt(3*3 + 4*4))

    float len2 = vec2.length();
    lpp::cout << "Length of vec2: " << len2 << "" << std::endl; 
	// Expected ~ 2.23607

    auto normVec = vec1.normalize();
    lpp::cout << "Normalized vec1 = (" << normVec.x << ", " << normVec.y << ")" << std::endl; 
    // Expected: Normalized vec1 = (0.6, 0.8)

    float dotProd = vec1.dot(vec2);
    lpp::cout << "Dot product of vec1 and vec2: " << dotProd << "" << std::endl; 
    // Expected: Dot product of vec1 and vec2: 11 (or 3*1 + 4*2)

    auto crossProd = vec1.cross(); //wrong
    lpp::cout << "Cross product of vec1: (" << crossProd.x << ", " << crossProd.y << ")" << std::endl; 
    // Expected: Cross product of vec1: (some_value, some_value)

    auto crossProdReal = vec1.cross(vec2);
    lpp::cout << "Cross product of vec1 and vect2: " << crossProdReal << std::endl;
    // Expected: Cross product of vec1 and vec2 : 2

   return 0;
}
