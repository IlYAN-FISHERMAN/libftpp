#include "../design_patterns/observer/observer.hh"
#include "../tests/tester.hh"
#include <iostream>
#include <random>

enum class EventType {
    EVENT_ONE,
    EVENT_TWO,
    EVENT_THREE
};

int myTestObserver() {
	{
		Observer<EventType, std::any> observer;
		observer.subscribe(EventType::EVENT_ONE, [](std::any data){
			std::cout << "Event One triggered with: " << std::any_cast<const char *>(data) << std::endl;
		});

		observer.subscribe(EventType::EVENT_TWO, [](std::any data){
			std::cout << "Event One triggered with: " << std::any_cast<double>(data) << std::endl;
		});

		observer.notify(EventType::EVENT_ONE, "bonjour");
		observer.notify(EventType::EVENT_TWO, 42.24);
	}
	{
		Observer<EventType, const char*, IoStat, double> big;

		big.subscribe(EventType::EVENT_ONE, [&](const char *str, IoStat io, double second){
			std::cout << "[EVENT1]: " << str << std::endl << io << std::endl << "second: " << second << std::endl;
		});
		IoStat io(1, "fdf", 10, 11);
		std::mt19937_64	mt{};
		std::random_device rd;
		mt.seed(rd());

		for (size_t it = 0, nb = (mt() % 1000) + 10; it < nb; it++)
			io.add(mt() % 1000, IoStat::Marks::READ);

		big.notify(EventType::EVENT_ONE, "fill data", io, 10.4);
	}
	return 0;
}

int testObserver() {
    Observer<EventType> observer;

    // Subscribe to EVENT_ONE
    observer.subscribe(EventType::EVENT_ONE, []() {
        std::cout << "Event One triggered" << std::endl;
    });

    // Subscribe first lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (First subscriber)" << std::endl;
    });

    // Subscribe second lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (Second subscriber)" << std::endl;
    });

    // Triggering EVENT_ONE
    std::cout << "Notify EVENT_ONE" << std::endl;
    observer.notify(EventType::EVENT_ONE);  // Output: "Event One triggered"

    // Triggering EVENT_TWO
    std::cout << "Notify EVENT_TWO" << std::endl;
    observer.notify(EventType::EVENT_TWO);  
    // Output: 
    // "Event Two triggered (First subscriber)"
    // "Event Two triggered (Second subscriber)"
    // The order may differ

    // Triggering EVENT_THREE (No subscriber)
    std::cout << "Notify EVENT_THREE" << std::endl;
    observer.notify(EventType::EVENT_THREE);  // Output: None, as there are no subscribers

    return 0;
}
