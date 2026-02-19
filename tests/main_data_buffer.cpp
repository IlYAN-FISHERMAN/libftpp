/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_data_buffer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:38 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 11:44:35 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../data_structures/data_structures.hpp"

class TestDataBufferObject {
public:
    int x;
    std::string y;
	double z;

    friend DataBuffer& operator<<(DataBuffer& p_buffer, const TestDataBufferObject& p_object) {
        p_buffer << p_object.x << p_object.y << p_object.z;
        return p_buffer;
    }

    friend DataBuffer& operator>>(DataBuffer& p_buffer, TestDataBufferObject& p_object) {
        p_buffer >> p_object.x >> p_object.y >> p_object.z;
        return p_buffer;
    }
};

int myTestDataBuffer() {
	return 0;
}

int testDataBuffer() {
    DataBuffer myBuffer;

    TestDataBufferObject obj1;
    obj1.x = 42;
    obj1.y = "Hello";
	obj1.z = 52.52;

    TestDataBufferObject obj2;
    obj2.x = 99;
    obj2.y = "World";
	obj2.z = 42.4242;

    myBuffer << obj1 << obj2;

    TestDataBufferObject deserializedObj1, deserializedObj2, deserializedObj3;

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
