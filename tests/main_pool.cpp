/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pool.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:18 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/17 14:40:58 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pool.hh"
#include "tester.hh"
#include <cmath>

class TestPoolObject {
public:
    TestPoolObject() { std::cout << "TestPoolObject default constructor" << std::endl; }
    TestPoolObject(int value) { std::cout << "TestPoolObject constructor with value [" << value << "]"<< std::endl;}
    TestPoolObject(int value, double lol) { std::cout << "TestPoolObject constructor with value [" << value << "] and" << lol << std::endl;}
    ~TestPoolObject() { std::cout << "TestPoolObject destructor" << std::endl; }

    void sayHello() const { std::cout << "Hello from TestPoolObject" << std::endl; }
    void saybye() const { std::cout << "bye from TestPoolObject" << std::endl; }
};

//--------------------------------------------
/// Get the current time
//--------------------------------------------
const char* getCurrentTime(){
	time_t timestamp;
	std::time(&timestamp);
	char *time = std::ctime(&timestamp);
	if (!time)
		exit (EXIT_FAILURE);
	time[strlen(time) - 1] = 0;
	return (time);
}

//--------------------------------------------
/// Main constructor
//--------------------------------------------
IoStat::IoStat(uint64_t fileId, std::string app, uid_t uid, gid_t gid) :
	_fileId(fileId), _app(app), _uid(uid), _gid(gid){
}

//--------------------------------------------
/// Destructor
//--------------------------------------------
IoStat::~IoStat(){}

//--------------------------------------------
/// Operator = overloaded
//--------------------------------------------
IoStat& IoStat::operator=(const IoStat &other){
	if (this != &other){
		_fileId = other._fileId;
		_app = other._app;
		_uid = other._uid;
		_gid = other._gid;
		_readMarks = other._readMarks;
		_writeMarks = other._writeMarks;
	}
	return (*this);
}

//--------------------------------------------
/// Constructor by copy constructor
//--------------------------------------------
IoStat::IoStat(const IoStat &other) :
	_fileId(other._fileId), _app(other._app), _uid(other._uid),
	_gid(other._gid), _readMarks(other._readMarks), _writeMarks(other._writeMarks){
}

//--------------------------------------------
/// Display the string given as parameter in
/// specific format with the current time
//--------------------------------------------
void	IoStat::printInfo(std::ostream &os, const char *msg){
	const char *time = getCurrentTime();
	os << IOSTAT_NAME << " [" << time << "]: " << msg << std::endl;
}

//--------------------------------------------
/// Display the string given as parameter in
/// specific format with the current time
//--------------------------------------------
void	IoStat::printInfo(std::ostream &os, const std::string &msg){
	const char *time = getCurrentTime();
	os << IOSTAT_NAME << " [" << time << "]: " << msg << std::endl;
}


//--------------------------------------------
/// Add bytes to the corresponding Read/Write deque
//--------------------------------------------
void IoStat::add(size_t bytes, IoStat::Marks enumMark){
	IoMark io(bytes);
	if (enumMark == Marks::READ)
		_readMarks.push_back(io);
	else if (enumMark == Marks::WRITE)
		_writeMarks.push_back(io);
}

//--------------------------------------------
/// Keep all I/O from the last N seconds
/// and cleans the rest
//--------------------------------------------
uint64_t IoStat::cleanOldsMarks(Marks enumMark, size_t seconds){
	if ((enumMark != Marks::READ && enumMark != Marks::WRITE)){
		if (io::IoStatDebug)
			IoStat::printInfo(std::cerr, "\033[031mNo marks found for\033[0m");
		return -1;
	}

	std::deque<IoMark> &mark = (enumMark == Marks::READ ? _readMarks : _writeMarks);

	std::deque<IoMark>::const_iterator begin = mark.begin();
	std::deque<IoMark>::const_iterator end = mark.end();

	struct timespec	currentTime;

	clock_gettime(CLOCK_REALTIME, &currentTime);

	// Calculates the iterator range corresponding to the last N seconds
	if (seconds > 0){
		for (std::deque<IoMark>::const_iterator it = begin; it != mark.end(); it++){
			if ((difftime(currentTime.tv_sec, it->io_time.tv_sec) < seconds))
				break;
			end = it;
		}
	}
	// checks if in case where seconds > 0 if there is something to delete
	if (seconds > 0 && end == mark.end()){
		if (io::IoStatDebug)
			printInfo(std::cout, "\033[031mNothing to clean\033[0m");
		return 1;
	}

	// Calculates the size of what will be erased
	size_t size = 0;
	if (seconds == 0){
		size = mark.size();
		mark.clear();
	} else{
		size = std::distance(begin, end);
		mark.erase(begin, end);
	}
	return size;
}

//--------------------------------------------
/// Calculate the write or read bandwidth
//--------------------------------------------
std::pair<double, double> IoStat::bandWidth(Marks enumMark, size_t *range, size_t seconds) const{
	if ((enumMark != Marks::READ && enumMark != Marks::WRITE) || seconds == 0){
		if constexpr (io::IoStatDebug){
			if (seconds == 0)
				IoStat::printInfo(std::cerr, "\033[031mCan't calculate bandwidth with 0s\033[0m");
			else
				IoStat::printInfo(std::cerr, "\033[031mNo marks found for\033[0m" + std::to_string(static_cast<uint8_t>(enumMark)));
		}
		return (std::pair(0, 0));
	}

	const std::deque<IoMark> &mark = (enumMark == Marks::READ ? _readMarks : _writeMarks);
	double avrg = 0;
	double deviation = 0;
	std::deque<IoMark>::const_iterator begin = mark.end();
	std::deque<IoMark>::const_iterator end = mark.end();
	struct timespec	currentTime;

	if (mark.empty())
		return (std::pair(0, 0));
	// Found the bandwidth iterator range
	clock_gettime(CLOCK_REALTIME, &currentTime);
	for (std::deque<IoMark>::const_iterator it = end; it != mark.begin();){
		it--;
		if ((difftime(currentTime.tv_sec, it->io_time.tv_sec) > seconds))
			break;
		begin = it;
	}
	if (range)
		*range = std::distance(begin, end);
	if (begin == end)
		return (std::pair(0, 0));

	// Calcule average
	for (std::deque<IoMark>::const_iterator it = begin; it < end; it++)
		avrg += static_cast<double>(it->bytes);
	if (std::distance(begin, end) > 0)
		avrg = avrg / std::distance(begin, end);

	// Calcule standard deviation
	if (std::distance(begin, end) - 1 > 0){
		for (std::deque<IoMark>::const_iterator it = begin; it < end; it++){
			deviation += std::pow(std::abs(it->bytes - avrg), 2);
		}
		deviation = std::sqrt(deviation / std::distance(begin, end));
	}

	return (std::pair<double, double>(avrg, deviation));
}

//--------------------------------------------
/// Get current uid
//--------------------------------------------
uid_t IoStat::getUid() const {return (_uid);}

//--------------------------------------------
/// Get current gid
//--------------------------------------------
gid_t IoStat::getGid() const {return (_gid);}

//--------------------------------------------
/// Get current app name
//--------------------------------------------
std::string IoStat::getApp() const {return (_app);}

//--------------------------------------------
/// Get the size of corresponding
/// READ or WRITE deck
//--------------------------------------------
ssize_t IoStat::getSize(Marks enumMark) const{
	if (enumMark == Marks::READ)
		return _readMarks.size();
	if (enumMark == Marks::WRITE)
		return _writeMarks.size();
	return 0;
}

//--------------------------------------------
/// Get the IOPS of corresponding
/// READ or WRITE deck
//--------------------------------------------
double IoStat::getIOPS(Marks enumMark, size_t seconds) const{
	if ((enumMark != Marks::READ && enumMark != Marks::WRITE)
		|| seconds == 0)
		return -1;

	double iops = 0;
	auto &mark= (enumMark == Marks::READ) ? _readMarks : _writeMarks;

	if (mark.size() == 0)
		return 0;

	struct timespec currentTime = mark.begin()->io_time;
	clock_gettime(CLOCK_REALTIME, &currentTime);

	for (std::deque<IoMark>::const_iterator it = mark.end(); it != mark.begin();){
		it--;
		if ((difftime(currentTime.tv_sec, it->io_time.tv_sec) > seconds))
			break;
		iops++;
	}
	iops /= seconds;

	return iops;
}

//--------------------------------------------
/// Overload operator << 
//--------------------------------------------
std::ostream& operator<<(std::ostream &os, const IoStat &other){
	std::pair<double, double> read = other.bandWidth(IoStat::Marks::READ);
	std::pair<double, double> write = other.bandWidth(IoStat::Marks::WRITE);
	os << "[IoStat bandwidth from last 10s] " << std::endl;
	os << std::fixed << std::setprecision(2) << C_BLUE
		<< "[READ][avrg: " << read.first << "][std: " << read.second
		<< "][s: " << other.getSize(IoStat::Marks::READ)
		<<  "][IOPS: " << other.getIOPS(IoStat::Marks::READ) << "]";
	os << " / ";
	os <<  "[WRITE][avrg: " << write.first << "][std: " << write.second
		<< "][s: " << other.getSize(IoStat::Marks::WRITE)
		<< "][IOPS: " << other.getIOPS(IoStat::Marks::WRITE)
		<< "]" << C_RESET;
	return os;
}

//--------------------------------------------
/// Overload operator << to print a entire multimap
//--------------------------------------------
std::ostream& operator<<(std::ostream &os, const std::unordered_multimap<uint64_t, const std::shared_ptr<IoStat> > &other){
	for (auto &it : other){
		os << C_GREEN << "┌─[" << C_CYAN << "IoMap" << C_GREEN << "]" << C_RESET;
		os << C_GREEN << "[" << C_CYAN << "id:" << it.first << C_GREEN << "]" << C_RESET;
		os << C_GREEN << "[" <<  C_CYAN << "app:"<< it.second->getApp() << C_GREEN << "]" << C_RESET;
		os << C_GREEN << "[" << C_CYAN << "uid:" << it.second->getUid() << C_GREEN << "]" << C_RESET;
		os << C_GREEN << "[" << C_CYAN << "gid:" << it.second->getGid() << C_GREEN << "]" << C_RESET;
		os << C_GREEN << "[" << C_CYAN << "sR:" << it.second->getSize(IoStat::Marks::READ)
			<< "/sW:"<< it.second->getSize(IoStat::Marks::WRITE) << C_GREEN << "]" << C_RESET;
		os << std::endl << C_GREEN << "└─[" << C_CYAN << "IoStat" << C_GREEN << "]" << C_RESET;
		os << C_WHITE << *it.second << C_RESET;
	}
	return os;
}

int myTestPool() {
	{
	std::cout << C_GREEN << "[Test 1]" << C_RESET << std::endl << std::endl;
	Pool<std::string> str;

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
	
    Pool<TestPoolObject> myPool;
	// Resize the pool to pre-allocate 5 objects
	// Should output the 5 "TestPoolObject constructor" (not in my version sorry)
    myPool.resize(5);

    // Acquire an object from the pool
    Pool<TestPoolObject>::Object obj1 = myPool.acquire(15);
    obj1->sayHello();  // Should output: "Hello from TestPoolObject"
	obj1->saybye();

    {
        // Acquire another object in a different scope
        Pool<TestPoolObject>::Object obj2 = myPool.acquire();
		std::cout << "addr of obj2: " << obj2.get() << std::endl;
        obj2->sayHello();  // Should also output: "Hello from TestPoolObject"
        // obj2 is released back to the pool when it goes out of scope
    }

    // Acquire another object; this should give us the object that obj2 pointed to
    Pool<TestPoolObject>::Object obj3 = myPool.acquire();
	std::cout << "addr of obj3: " << obj3.get() << std::endl;
    Pool<TestPoolObject>::Object obj4 = myPool.acquire();
	std::cout << "addr of obj4: " << obj4.get() << std::endl;
    obj3->sayHello();  // Should output: "Hello from TestPoolObject"

    // obj1 and obj3 are released back to the pool when the program ends
	}
	std::cout << std::endl << C_GREEN << "[Test 1 succedded]" << C_RESET << std::endl << std::endl;
	


	// Second part
	{
	std::cout << std::endl << C_GREEN << "[Test 2]" << C_RESET << std::endl << std::endl;
		Pool<TestPoolObject> pool;
		pool.resize(10);
		std::vector<Pool<TestPoolObject>::Object> tmp;

		{
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
				auto obj3(pool.acquire());
			} catch (std::logic_error &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}

			try{
				std::cout << std::endl << "part2" << std::endl << std::endl;

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

			} catch (std::logic_error &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}

			try{
				std::cout << std::endl << "part3" << std::endl << std::endl;

				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				pool.resize(100);
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				while (!tmp.empty())
					tmp.pop_back();
				std::cout << std::endl;
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				while (pool.capacity() > 0)
					tmp.push_back(pool.acquire());
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
				while (!tmp.empty())
					tmp.pop_back();
				std::cout << "capacity: " << pool.capacity() << std::endl;
				std::cout << "max: " << pool.maxSize() << std::endl;
				std::cout << "used: " << pool.size() << std::endl << std::endl;
			} catch (std::logic_error &e){
				std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
			}
		}
		std::cout << std::endl << C_GREEN << "[Test 2 succedded]" << C_RESET << std::endl << std::endl;
	}

	return 0;
}

int benchPoolTest(){
	int len = 100000000;

	for (auto it = 0; it < len; it++){
		auto *tmp = new IoStat(1, "eos", 1, 1);
		delete tmp;
	}

	// Pool<IoStat> io;
	// io.resize(len);
	// for (auto it = 0; it < len; it++){
	// 	auto tmp = io.acquire(1, "eos", 1, 1);
	// }

	return 0;
}


int testPool() {
    // Create a Pool for TestPoolObject
	Pool<TestPoolObject> myPool;

    // obj1 and obj3 are released back to the pool when the program ends
	myPool.resize(5);

     // Acquire an object from the pool
	 Pool<TestPoolObject>::Object obj1 = myPool.acquire(15);
	obj1->sayHello();  // Should output: "Hello from TestPoolObject"

	{
         // Acquire another object in a different scope
    	Pool<TestPoolObject>::Object obj2 = myPool.acquire();
    	obj2->sayHello();  // Should also output: "Hello from TestPoolObject"
         // obj2 is released back to the pool when it goes out of scope
    }

	// Acquire another object; this should give us the object that obj2 pointed to
    Pool<TestPoolObject>::Object obj3 = myPool.acquire();
    obj3->sayHello();  // Should output: "Hello from TestPoolObject"

    // obj1 and obj3 are released back to the pool when the program ends
	return 0;
}
