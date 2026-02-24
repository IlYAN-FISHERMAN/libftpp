/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_memento.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:49:43 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 15:52:14 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

class TestMementoClass : public Memento {
    friend class Memento;

public:
    int x;
    std::string y;
	IoStat io{1, "eos", 1, 1};

private:
    void _saveToSnapshot(Snapshot& snapshotToFill) const override {
        snapshotToFill << x << y << io;
    }

    void _loadFromSnapshot(Snapshot& snapshot) override {
        snapshot >> x >> y >> io;
    }

};

int myTestMemento() {
	std::mt19937_64	mt{};
	std::random_device rd;
	mt.seed(rd());
	{
		std::cout << C_GREEN << "[TEST 1]" << C_RESET << std::endl;
		TestMementoClass myObject;

		std::mt19937_64	mt{};
		std::random_device rd;
		mt.seed(rd());
		std::cout << "min: " << mt.min() << ", max: " << mt.max() << std::endl;
		myObject.x = 42;
		myObject.y = "Hello";
		for (size_t it = 0, nb = (mt() % 1000) + 10; it < nb; it++)
			myObject.io.add(mt() % 1000, IoStat::Marks::READ);
		std::cout << "first state: x = " << myObject.x << ", y = " << myObject.y << "\n" << myObject.io << std::endl;

		// Save the current state
		TestMementoClass::Snapshot savedState = myObject.save();

		// Modify the object
		myObject.x = 100;
		myObject.y = "World";
		for (size_t it = 0, nb = (mt() % 1000) + 10; it < nb; it++)
			myObject.io.add(mt() % 1000, IoStat::Marks::READ);

		// Output the modified object
		// Expected Output: "Current state: x = 100, y = World"
		std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << "\n" << myObject.io << std::endl;

		// Restore the object to its saved state
		try {
			myObject.load(savedState);
		} catch (std::bad_any_cast &e){
			std::cerr << e.what() << std::endl;
			std::cout << "but this is normal" << std::endl;
		}
	 
		// Output the restored object
		// Expected Output: "Restored state: x = 42, y = Hello"
		std::cout << "Restored state: x = " << myObject.x << ", y = " << myObject.y << "\n" << myObject.io << std::endl;
	}
	{
		std::cout << C_GREEN << "[TEST 1]" << C_RESET << std::endl;
		TestMementoClass myObject;
		myObject.io = IoStat(1, "fdf", 10, 11);
		myObject.x = 42;
		myObject.y = "bonjour a tous";

		for (size_t it = 0, nb = (mt() % 1000) + 10; it < nb; it++)
			myObject.io.add(mt() % 1000, IoStat::Marks::READ);
		myObject.y = "small test";
		std::cout << "IO: " << myObject.io << std::endl;

		auto saveBefore = myObject.save();
		myObject.io.cleanOldsMarks(IoStat::Marks::READ, 0);
		std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << "\n" << myObject.io << std::endl;
		std::cout << "SAVE" << std::endl;
		myObject.load(saveBefore);
		std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << "\n" << myObject.io << std::endl;
	}
	return 0;
}

int testMemento() {
    TestMementoClass myObject;
    myObject.x = 42;
    myObject.y = "Hello";

    // Save the current state
    TestMementoClass::Snapshot savedState = myObject.save();

    // Modify the object
    myObject.x = 100;
    myObject.y = "World";

    // Output the modified object
    // Expected Output: "Current state: x = 100, y = World"
    std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

    // Restore the object to its saved state
    myObject.load(savedState);
 
    // Output the restored object
	// Expected Output: "Restored state: x = 42, y = Hello"
    std::cout << "Restored state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

    return 0;
}
