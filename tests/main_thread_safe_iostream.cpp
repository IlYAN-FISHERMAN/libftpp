#include <iostream>
#include <thread>
#include <vector>
#include "../iostream/thread_safe_iostream.hh"

void printNumbers(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);

    for (int i = 0; i <= 5; ++i) {
        threadSafeCout << "Number: " << i << std::endl;
		// std::this_thread::sleep_for(std::chrono::seconds(1));
    }
	if (threadSafeCout.prefix() == "[Thread 8]: ")
		threadSafeCout << "bonjour a tous je suis le huitieme thread" << std::endl;
}

int myTestThreadSafeIostream(){
	std::vector<std::thread> v;
	for (size_t it = 0; it < 10; it++)
		v.push_back(std::thread(printNumbers, std::string("[Thread ") + std::to_string(it) + "]: "));

	for (auto &it : v)
		it.join();

	return 0;
}

int testThreadSafeIostream() {
    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    std::thread thread1(printNumbers, prefix1);
    std::thread thread2(printNumbers, prefix2);

    thread1.join();
    thread2.join();

    return 0;
}
