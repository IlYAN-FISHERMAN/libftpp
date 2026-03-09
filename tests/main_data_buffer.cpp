/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_data_buffer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:38 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:13:07 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

class Testdata_bufferObject {
public:
    int x;
    std::string y;
	double z;

    friend lpp::data_buffer& operator<<(lpp::data_buffer& p_buffer, const Testdata_bufferObject& p_object) {
        p_buffer << p_object.x << p_object.y << p_object.z;
        return p_buffer;
    }

    friend lpp::data_buffer& operator>>(lpp::data_buffer& p_buffer, Testdata_bufferObject& p_object) {
        p_buffer >> p_object.x >> p_object.y >> p_object.z;
        return p_buffer;
    }
};

int myTestdata_buffer() {
	return 0;
}

int testdata_buffer() {
    lpp::data_buffer myBuffer;

    Testdata_bufferObject obj1;
    obj1.x = 42;
    obj1.y = "Hello";
	obj1.z = 52.52;

    Testdata_bufferObject obj2;
    obj2.x = 99;
    obj2.y = "World";
	obj2.z = 42.4242;

    myBuffer << obj1 << obj2;

    Testdata_bufferObject deserializedObj1, deserializedObj2, deserializedObj3;

    // This should work as expected
    try {
        myBuffer >> deserializedObj1 >> deserializedObj2;
        std::cout << "Deserialized obj1: x = " << deserializedObj1.x << ", y = " << deserializedObj1.y << ", z = " <<  deserializedObj1.z << std::endl;
        std::cout << "Deserialized obj2: x = " << deserializedObj2.x << ", y = " << deserializedObj2.y << ", z = " <<  deserializedObj2.z << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // This should throw an exception because there are no more objects to deserialize
    try {
        myBuffer >> deserializedObj3;
        std::cout << "Deserialized obj3: x = " << deserializedObj3.x << ", y = " << deserializedObj3.y << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;  // This line should be executed
    }

    return 0;
}
