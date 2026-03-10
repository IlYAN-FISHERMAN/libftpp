/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pool.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:18 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/10 15:38:43 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

class TestpoolObject {
public:
    TestpoolObject() { std::cout << "TestpoolObject default constructor" << std::endl; }
    TestpoolObject(int value) { std::cout << "TestpoolObject constructor with value [" << value << "]"<< std::endl;}
    TestpoolObject(int value, double lol) { std::cout << "TestpoolObject constructor with value [" << value << "] and" << lol << std::endl;}
    ~TestpoolObject() { std::cout << "TestpoolObject destructor" << std::endl; }

    void sayHello() const { std::cout << "Hello from TestpoolObject" << std::endl; }
    void saybye() const { std::cout << "bye from TestpoolObject" << std::endl; }
};

int myTestPool() {
	{
	std::cout << C_GREEN << "[Test 1]" << C_RESET << std::endl << std::endl;
	lpp::pool<std::string> str;

	str.resize(1);
	try{
		// try to create 3 objet, should throw a exception
		auto tmp = str.acquire("bonjour");
		std::cout << "tmp obj: " << *str.acquire("hello") << std::endl;
		std::cout << "tmp obj: " << *str.acquire("hello") << std::endl;
	} catch (std::out_of_range &e){
		std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
	// resize with only 2 slot
	str.resize(2);
	try{
		// should not throw anything
		auto tmp = str.acquire("bonjour");
		std::cout << *str.acquire("hello") << std::endl;
		std::cout << *str.acquire("hello") << std::endl;
		std::cout << "print with surcharge of * operator: " << *tmp << std::endl << std::endl;
	} catch (std::out_of_range &e){
		std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
	
    lpp::pool<TestpoolObject> mypool;
	// Resize the pool to pre-allocate 5 objects
	// Should output the 5 "TestpoolObject constructor" (not in my version sorry)
    mypool.resize(5);

    // Acquire an object from the pool
    lpp::pool<TestpoolObject>::Object obj1 = mypool.acquire(15);
    obj1->sayHello();  // Should output: "Hello from TestpoolObject"
	obj1->saybye();

    {
        // Acquire another object in a different scope
        lpp::pool<TestpoolObject>::Object obj2 = mypool.acquire();
		std::cout << "addr of obj2: " << obj2.get() << std::endl;
        obj2->sayHello();  // Should also output: "Hello from TestpoolObject"
        // obj2 is released back to the pool when it goes out of scope
    }

    // Acquire another object; this should give us the object that obj2 pointed to
    lpp::pool<TestpoolObject>::Object obj3 = mypool.acquire();
	std::cout << "addr of obj3: " << obj3.get() << std::endl;
    lpp::pool<TestpoolObject>::Object obj4 = mypool.acquire();
	std::cout << "addr of obj4: " << obj4.get() << std::endl;
    obj3->sayHello();  // Should output: "Hello from TestpoolObject"

    // obj1 and obj3 are released back to the pool when the program ends
	}
	std::cout << std::endl << C_GREEN << "[Test 1 succedded]" << C_RESET << std::endl << std::endl;
	


	// Second part
	{
	std::cout << std::endl << C_GREEN << "[Test 2]" << C_RESET << std::endl << std::endl;
		lpp::pool<TestpoolObject> pool;
		pool.resize(10);
		std::vector<lpp::pool<TestpoolObject>::Object> tmp;

		{
			std::cout << "[TEST 2.0]" << std::endl << std::endl;
			try{
				{
					{
						auto obj2(pool.acquire());
						std::cout << obj2.get() << std::endl;
						tmp.push_back(std::move(obj2));
					}
					std::cout << tmp[0].get() << std::endl;
				}
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				for (auto it = 0; it < 4; it++)
					tmp.push_back(pool.acquire());
				pool.resize(2);
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				auto obj3(pool.acquire()); // ERROR
				return -1;
			} catch (std::exception &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}

			try{
				std::cout << std::endl << "[TEST 2.1]" << std::endl << std::endl;

				for (auto it = 0; it < 4; it ++)
					tmp.pop_back();
				pool.resize(3);
					{
						auto obj2(pool.acquire());
						std::cout << obj2.get() << std::endl;
						tmp.push_back(std::move(obj2));
					}
					std::cout << tmp.back().get() << std::endl;
				while (pool.capacity() > 0)
					tmp.push_back(pool.acquire());
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				pool.resize(35);
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				while (pool.capacity() > 3)
					tmp.push_back(pool.acquire());
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				pool.resize(5);
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				{
					auto obj2(pool.acquire());
					std::cout << obj2.get() << std::endl;
					tmp.push_back(std::move(obj2));
				}
				std::cout << "error if you see this" << std::endl;
				return -1;

			} catch (std::exception &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}

			try{
				std::cout << "[TEST 2.2]" << std::endl;
				std::cout << "\tcapacity: " << pool.capacity() << std::endl;
				std::cout << "\tmax: " << pool.maxSize() << std::endl;
				std::cout << "\tused: " << pool.size() << std::endl << std::endl;

				std::cout << "[resize 100 for pool]" << std::endl;
				pool.resize(100);
				std::cout << "[POOL]" << std::endl;
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				std::cout << "[CLEAR DATA]" << std::endl;
				while (!tmp.empty())
					tmp.pop_back();
				std::cout << std::endl;
				std::cout << "[POOL]" << std::endl;
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;

				std::cout << std::endl << "[use all data from pool]" << std::endl;
				std::cout << "[POOL]" << std::endl;
				while (pool.capacity() > 0)
					tmp.push_back(pool.acquire());
				std::cout << "[POOL]" << std::endl;
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				while (!tmp.empty())
					tmp.pop_back();
				std::cout << std::endl << "[POOL]" << std::endl;
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
			} catch (std::exception &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}
		}
		{
			try
			{
			}catch(std::exception &e){
				std::cout << e.what() << std::endl;
			}
		}
		std::cout << std::endl << C_GREEN << "[Test 2 succedded]" << C_RESET << std::endl << std::endl;
	}

	return 0;
}

int deleteTestPool(){
	try{
		lpp::pool<TestpoolObject>* pool = new lpp::pool<TestpoolObject>();
		pool->resize(10);
		lpp::pool<TestpoolObject>::Object obj(pool->acquire());
		delete pool;
		obj->sayHello();
		std::cout << "capacity: " << pool->capacity() << std::endl;
		std::cout << "max: " << pool->maxSize() << std::endl;
		std::cout << "used: " << pool->size() << std::endl << std::endl;
	} catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}

	return 0;
}

int benchPoolTest(){
	int len = 100000000;

	for (auto it = 0; it < len; it++){
		auto *tmp = new IoStat(1, "eos", 1, 1);
		delete tmp;
	}

	// pool<IoStat> io;
	// io.resize(len);
	// for (auto it = 0; it < len; it++){
	// 	auto tmp = io.acquire(1, "eos", 1, 1);
	// }

	return 0;
}


int testPool() {
    // Create a pool for TestpoolObject
	lpp::pool<TestpoolObject> mypool;
	lpp::unique_chrono chrono("default pool");

    // obj1 and obj3 are released back to the pool when the program ends
	mypool.resize(5);

     // Acquire an object from the pool
	 lpp::pool<TestpoolObject>::Object obj1 = mypool.acquire(15);
	 obj1->sayHello();  // Should output: "Hello from TestpoolObject"
	{
         // Acquire another object in a different scope
    	lpp::pool<TestpoolObject>::Object obj2 = mypool.acquire();
    	obj2->sayHello();  // Should also output: "Hello from TestpoolObject"
         // obj2 is released back to the pool when it goes out of scope
    }

	// Acquire another object; this should give us the object that obj2 pointed to
    lpp::pool<TestpoolObject>::Object obj3 = mypool.acquire();
    obj3->sayHello();  // Should output: "Hello from TestpoolObject"

    // obj1 and obj3 are released back to the pool when the program ends
	return 0;
}
