// Performance Assignment.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>


class TIMER
{
	LARGE_INTEGER t_;

	__int64 current_time_;

public:
	TIMER()	// Default constructor. Initialises this timer with the current value of the hi-res CPU timer.
	{
		QueryPerformanceCounter(&t_);
		current_time_ = t_.QuadPart;
	}

	TIMER(const TIMER &ct)	// Copy constructor.
	{
		current_time_ = ct.current_time_;
	}

	TIMER& operator=(const TIMER &ct)	// Copy assignment.
	{
		current_time_ = ct.current_time_;
		return *this;
	}

	TIMER& operator=(const __int64 &n)	// Overloaded copy assignment.
	{
		current_time_ = n;
		return *this;
	}

	~TIMER() {}		// Destructor.

	static __int64 get_frequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	__int64 get_time() const
	{
		return current_time_;
	}

	void get_current_time()
	{
		QueryPerformanceCounter(&t_);
		current_time_ = t_.QuadPart;
	}

	inline bool operator==(const TIMER &ct) const
	{
		return current_time_ == ct.current_time_;
	}

	inline bool operator!=(const TIMER &ct) const
	{
		return current_time_ != ct.current_time_;
	}

	__int64 operator-(const TIMER &ct) const		// Subtract a TIMER from this one - return the result.
	{
		return current_time_ - ct.current_time_;
	}

	inline bool operator>(const TIMER &ct) const
	{
		return current_time_ > ct.current_time_;
	}

	inline bool operator<(const TIMER &ct) const
	{
		return current_time_ < ct.current_time_;
	}

	inline bool operator<=(const TIMER &ct) const
	{
		return current_time_ <= ct.current_time_;
	}

	inline bool operator>=(const TIMER &ct) const
	{
		return current_time_ >= ct.current_time_;
	}
};

struct Entry
{
	std::string sessionId, ipaddress, browser;
	std::map<std::string, std::string> pages;	//key == path to page, value is time spent in seconds. 
};

class fileManager
{
public:
	void openFile(std::string fileName = "testdata_short.txt")
	{
		std::string fileDirectory = "C:/Users/b6035780/Desktop/Performance Assignment/data/";
		fs.open(fileDirectory + fileName);
	}

	void parseFile()
	{
		std::string line, page;

		while (std::getline(fs, line))
		{
			Entry e;
			line = line.substr(18, line.length());																			//Removes fluff at start of line - <entry><sessionid>
			e.sessionId = line.substr(0, line.find("</sessionid>"));														//extract session Id
			line = line.substr(line.find("</sessionid>") + 23, line.length());												//remove tags again - </sessionid><ipaddress>
			e.ipaddress = line.substr(0, line.find("</ipaddress>"));														//extract IP
			line = line.substr(line.find("</ipaddress>") + 21, line.length());												//remove tags
			e.browser = line.substr(0, line.find("</browser>"));															//extract browser
			line = line.substr(line.find("</browser>") + 16, line.length());

			while (line != "y>")																							//While loop goes through every path visited in the session. When no pages are left, y> (from </entry>) is all thats left
			{
				page = line.substr(0, line.find("</path>"));																//Path name is temporarily stored in string while the time is extracted
				line = line.substr(line.find("</path>") + 13, line.length());												//remove tags
				e.pages[page] = line.substr(0, line.find("</time>"));														//Adds time (as unconverted string) to pages map 
				line = line.substr(line.find("</time>") + 13, line.length());												//removes tags
			}
			entries.push_back(e);																							//adds entry to vector
		}
	}

	~fileManager() { fs.close(); }
private:
	std::fstream fs;
	std::vector<Entry> entries;
};


int main()
{
	// Application starts here...

	// Time the application's execution time.
	TIMER start;	// DO NOT CHANGE THIS LINE. Timing will start here.

					//--------------------------------------------------------------------------------------
					// Insert your code from here...
	fileManager fm;
	fm.openFile();
	fm.parseFile();




					//-------------------------------------------------------------------------------------------------------
					// How long did it take?...   DO NOT CHANGE FROM HERE...

	TIMER end;

	TIMER elapsed;

	elapsed = end - start;

	__int64 ticks_per_second = start.get_frequency();

	// Display the resulting time...

	double elapsed_seconds = (double)elapsed.get_time() / (double)ticks_per_second;

	std::cout.precision(17);
	std::cout << "Elapsed time (seconds): " << std::fixed << elapsed_seconds;
	std::cout << std::endl;
	std::cout << "Press a key to continue" << std::endl;

	char c;
	std::cin >> c;

    return 0;
}


