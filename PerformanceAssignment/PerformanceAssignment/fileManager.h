#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

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
		std::string fileDirectory = "F:/MyWork/Effective C++/Performance Assignment/PerformanceAssignment/Data/";
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

	void convertToJson()
	{
		std::fstream fs;
		std::string fileDirectory = "F:/MyWork/Effective C++/Performance Assignment/PerformanceAssignment/Data/";
		fs.open(fileDirectory + "data.json", std::fstream::out);

		fs << "{" << "\n";
		fs << "\"entries\": [" << "\n";
		std::for_each(entries.begin(), entries.end(), [&fs](Entry e)
		{
			fs << "{" << "\n";
			fs << "\"SessionId\": \"" << e.sessionId << "\"," << "\n";
			fs << "\"ipAddress\": \"" << e.ipaddress << "\"," << "\n";
			fs << "\"browser\": \"" << e.browser << "\"," << "\n";

			fs << "\"pages\": [" << "\n";
			std::for_each(e.pages.begin(), e.pages.end(), [&fs](std::pair<std::string, std::string> p)
			{
				fs << "{" << "\n";
				fs << "\"Page\": \"" << p.first << "\"," << "\n";
				fs << "\"Time\": \"" << p.second << "\"," << "\n";
				fs << "}," << "\n";
			});

			fs << "]" << "\n" << "}," << "\n";
		});
	}

	~fileManager() { fs.close(); }
private:
	std::fstream fs;
	std::vector<Entry> entries;
};

#endif
