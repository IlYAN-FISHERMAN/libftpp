/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 08:14:49 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/20 19:52:50 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <sys/types.h>
#include <iostream>
#include <ctime>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <iomanip>
#include <utility>

int testPool();
int deleteTestPool();
int benchPoolTest();
int myTestPool();

int testDataBuffer();
int myTestDataBuffer();

int testMemento();
int myTestMemento();

int testObserver();
int myTestObserver();

//--------------------------------------------
/// The current name of the class when us
/// printInfo function
//--------------------------------------------
#define IOSTAT_NAME "IoStat"

//--------------------------------------------
/// Main structure stored in IoStat
/// Keep the time of class creation
//--------------------------------------------
struct IoMark {
	struct timespec io_time;
    size_t bytes;

	//--------------------------------------------
	/// Main contructor
	//--------------------------------------------
    IoMark(size_t bytes) : bytes(bytes){
		clock_gettime(CLOCK_REALTIME, &io_time);
	}

	//--------------------------------------------
	/// Default contructor
	//--------------------------------------------
    IoMark() : bytes(0){
		clock_gettime(CLOCK_REALTIME, &io_time);
	}
};

class IoStat {
		public: enum class Marks : uint8_t{
			READ,
			WRITE
		};

	private:
		//--------------------------------------------
		/// Each IoStat class keeps track of exactly
		/// where it came from. The file ID, the name
		/// of the app it came from, the user ID and
		/// the group ID
		//--------------------------------------------
		uint64_t	_fileId;
		std::string	_app;
		uid_t		_uid;
		gid_t		_gid;

		//--------------------------------------------
		/// Read deque that keeps track all of read
		/// operations on the _fileId
		//--------------------------------------------
		std::deque<IoMark> _readMarks;

		//--------------------------------------------
		/// Write deque that keeps track all of written
		/// operations on the _fileId
		//--------------------------------------------
		std::deque<IoMark> _writeMarks;


		//--------------------------------------------
		/// Deleted default constructor
		//--------------------------------------------
		IoStat() = delete;

	public:
		//--------------------------------------------
		/// Orthodoxe canonical form
		//--------------------------------------------
	
		//--------------------------------------------
		/// Destructor
		//--------------------------------------------
		~IoStat();

		//--------------------------------------------
		/// Constructor by copy constructor
		//--------------------------------------------
		IoStat(const IoStat &other);

		//--------------------------------------------
		/// Overload the operator =
		//--------------------------------------------
		IoStat& operator=(const IoStat &other);

		//--------------------------------------------
		/// @brief Main constructor to initialize the class
		///
		/// @param	fileId	Tracked file id
		/// @param	app		Name of the application where
		/// the file is linked
		/// @param	uid		Id of the user
		/// @param	gid		Id of the group
		//--------------------------------------------
		IoStat(uint64_t fileId, std::string app, uid_t uid, gid_t gid);

		//--------------------------------------------
		/// @brief Add bytes to the corresponding Read/Write deque
		/// and records its execution timestamp
		///
		/// @param	rBytes Number of bytes read
		/// @param	enumMark READ or WRITE variable comes
		/// from the IoStat::Marks enumerator
		//--------------------------------------------
		void add(size_t bytes, IoStat::Marks enumMark);

		//--------------------------------------------
		/// @brief Keep all I/O from the last N seconds.
		///
		/// @details
		/// The function takes as parameters the number of
		/// seconds corresponding to the number of seconds
		/// of I/O which will be kept, the rest will be erased.
		/// 
		/// @param	enumMark READ or WRITE variable comes
		/// from the IoStat::Marks enumerator
		/// @param seconds(optional) The number of last seconds of
		/// I/O that will be kept (by default - 10s)
		///
		/// @return -1 If the enumerator is incorrect
		/// @return 1 If there is nothing to delete
		/// @return size Else returns the size of what was erased
		//--------------------------------------------
		uint64_t cleanOldsMarks(Marks enumMark, size_t seconds = 10);


		//--------------------------------------------
		/// @brief Calculate the write or read bandwidth
		///
		/// @details
		/// The function calculates the read or
		/// write bandwidth (depending on the "enumMark" parameter)
		/// of the last N seconds (by default - 10s)
		///
		/// The function thus calculates the average and
		/// the standard deviation of the range of data found
		/// 
		/// @param	enumMark READ or WRITE variable comes
		/// from the IoStat::Marks enumerator
		/// @param	range Sets the "range" variable to the
		/// number of elements that were found
		/// (can be set to NULL in which case the parameter
		/// is ignored)
		/// @param	seconds(optional) over how many seconds
		/// from now the function should calculate
		///
		/// @return std::pair<double, double> fisrt is the
		/// average, the second is the standard deviation 
		//--------------------------------------------
		std::pair<double, double> bandWidth(Marks EnumMark, size_t *range = NULL, size_t seconds = 10) const;

		//--------------------------------------------
		/// @brief Calculate the write or read IOPS
		///
		/// @details
		/// The function calculates the read or
		/// write IOPS (depending on the "enumMark" parameter)
		/// of the last N seconds (by default - 10s)
		///
		/// @param	enumMark READ or WRITE variable comes
		/// from the IoStat::Marks enumerator
		/// @param seconds(optional) over how many seconds
		/// from now the function should calculate
		///
		/// @return -1 If the enumerator is incorrect
		/// @return double The IOPS
		//--------------------------------------------
		double getIOPS(Marks enumMark, size_t seconds = 10) const;

		//--------------------------------------------
		/// Static function
		/// @brief Displays the string given as a parameter
		/// in a format corresponding to the class with the
		/// current timestamp
		///
		/// @param	os The output stream
		/// @param	msg The message to display
		//--------------------------------------------
		static void	printInfo(std::ostream &os, const char *msg);

		//--------------------------------------------
		/// Static function
		/// @brief Displays the string given as a parameter
		/// in a format corresponding to the class with the
		/// current timestamp
		///
		/// @param	os The output stream
		/// @param	msg The message to display
		//--------------------------------------------
		static void	printInfo(std::ostream &os, const std::string &msg);

		//--------------------------------------------
		/// Get current uid
		//--------------------------------------------
		uid_t getUid() const;

		//--------------------------------------------
		/// Get current gid
		//--------------------------------------------
		gid_t getGid() const;

		//--------------------------------------------
		/// Get current app name
		//--------------------------------------------
		std::string getApp() const;

		//--------------------------------------------
		/// @brief Get the size of corresponding
		/// READ or WRITE deck
		///
		/// @param	enumMark READ or WRITE variable comes
		/// from the IoStat::Marks enumerator
		//--------------------------------------------
		ssize_t getSize(Marks enumMark) const;
};


//--------------------------------------------
/// @brief Overload operator << to print the average
/// and standard deviation of the last 10 seconds
//--------------------------------------------
std::ostream& operator<<(std::ostream &os, const IoStat &other);

//--------------------------------------------
/// @brief Overload operator << to print
/// the entire multimap
//--------------------------------------------
std::ostream& operator<<(std::ostream &os, const std::unordered_multimap<uint64_t, std::shared_ptr<IoStat> > &other);

//--------------------------------------------
/// Namespace
///
/// If debug is "true" all debug phases of
/// the corresponding class will be displayed
//--------------------------------------------
namespace io {
	constexpr bool IoStatDebug = false;
	constexpr bool IoMapDebug = false;
	constexpr bool IoMarkDebug = false;
	constexpr bool IoAggregateMapDebug = false;
	constexpr bool IoAggregateDebug = false;

	//--------------------------------------------
	/// Enumerator that allows to keep the context
	/// of UID or GID type of a variable
	//--------------------------------------------
	enum class TYPE {
		UID,
		GID
	};
}

//--------------------------------------------
/// File that defines all the colors for the terminal
//--------------------------------------------
# define C_RED "\001\033[0;31m\002"
# define C_GREEN "\001\033[0;32m\002"
# define C_YELLOW "\001\033[0;33m\002"
# define C_BLUE "\001\033[1;34m\002"
# define C_MAGENTA "\001\033[1;35m\002"
# define C_CYAN "\001\033[36m\002"
# define C_BLACK "\001\033[30m\002"
# define C_WHITE "\001\033[37m\002"
# define C_RESET "\001\033[0m\002"

//--------------------------------------------
/// Text effects definition
//--------------------------------------------
# define C_BOLD "\001\033[1m\002"
# define C_UNDERLINE "\001\033[4m\002"
# define C_BLINK "\001\033[5m\002"
# define C_REVERSE "\001\033[7m\002"
# define C_INVISIBLE "\001\033[8m\002"
# define C_STRIKETHROUGH "\001\033[9m\002"
# define C_CLEAR "\001\033[2J\001\033[1;1H"

//--------------------------------------------
/// Background colors definition
//--------------------------------------------
# define C_BBLACK "\001\033[40m\002"
# define C_BWHITE "\001\033[47m\002"
# define C_BRED "\001\033[41m\002"
# define C_BGREEN "\001\033[42m\002"
# define C_BYELLOW "\001\033[43m\002"
# define C_BBLUE "\001\033[44m\002"
# define C_BMAGENTA "\001\033[45m\002"
# define C_BCYAN "\001\033[46m\002"
