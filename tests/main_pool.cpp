#include <iostream>
#include "../include/pool.hh"

class TestObject {
public:
    TestObject() { std::cout << "TestObject default constructor" << std::endl; }
    TestObject(int value) { std::cout << "TestObject constructor with value [" << value << "]"<< std::endl;}
    TestObject(int value, double lol) { std::cout << "TestObject constructor with value [" << value << "] and" << lol << std::endl;}
    ~TestObject() { std::cout << "TestObject destructor" << std::endl; }

    void sayHello() const { std::cout << "Hello from TestObject" << std::endl; }
    void saybye() const { std::cout << "bye from TestObject" << std::endl; }
};

int myTestPool() {
    // Create a Pool for TestObject
    Pool<TestObject> myPool;
	Pool<std::string> str;
	Pool<std::vector<std::string>> vec;

	str.resize(1);
	try{
		// try to create 3 objet, should throw a exception
		auto tmp = str.acquire("bonjour");
		std::cout << "tmp obj: " << *str.acquire("hello") << std::endl;
		std::cout << "tmp obj: " << *str.acquire("hello") << std::endl;
		std::cout << "print with surcharge of * operator " << *tmp << std::endl;
	} catch (std::out_of_range &e){
		std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
	// resize with only 2 slot
	str.resize(2);
	try{
		auto tmp = str.acquire("bonjour");
		std::cout << *str.acquire("hello") << std::endl;
		std::cout << *str.acquire("hello") << std::endl;
		std::cout << *tmp << std::endl;
		// Resize the pool to pre-allocate 5 objects
		// Should output the 5 "TestObject constructor"
	} catch (std::out_of_range &e){
		std::cout << e.what() << std::endl;
	}
	
	// Resize the pool to pre-allocate 5 objects
	// Should output the 5 "TestObject constructor"
    myPool.resize(5);

    // Acquire an object from the pool
    Pool<TestObject>::Object obj1 = myPool.acquire(15);
    obj1->sayHello();  // Should output: "Hello from TestObject"
	obj1->saybye();

    {
        // Acquire another object in a different scope
        Pool<TestObject>::Object obj2 = myPool.acquire();
		std::cout << obj2.get() << std::endl;
        obj2->sayHello();  // Should also output: "Hello from TestObject"
        // obj2 is released back to the pool when it goes out of scope
    }

    // Acquire another object; this should give us the object that obj2 pointed to
    Pool<TestObject>::Object obj3 = myPool.acquire();
	std::cout << obj3.get() << std::endl;
    Pool<TestObject>::Object obj4 = myPool.acquire();
	std::cout << obj4.get() << std::endl;
    obj3->sayHello();  // Should output: "Hello from TestObject"

    // obj1 and obj3 are released back to the pool when the program ends
    return 0;
}
int testPool() {
    // Create a Pool for TestObject
	Pool<TestObject> myPool;

    // obj1 and obj3 are released back to the pool when the program ends
	myPool.resize(5);

     // Acquire an object from the pool
	 Pool<TestObject>::Object obj1 = myPool.acquire(15);
	obj1->sayHello();  // Should output: "Hello from TestObject"

	{
         // Acquire another object in a different scope
    	Pool<TestObject>::Object obj2 = myPool.acquire();
    	obj2->sayHello();  // Should also output: "Hello from TestObject"
         // obj2 is released back to the pool when it goes out of scope
    }

	// Acquire another object; this should give us the object that obj2 pointed to
    Pool<TestObject>::Object obj3 = myPool.acquire();
    obj3->sayHello();  // Should output: "Hello from TestObject"

    // obj1 and obj3 are released back to the pool when the program ends
	return 0;
}
