#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <windows.h>
#include <imap.h>
#include <boost/filesystem.hpp>
#include <boost/container/map.hpp>

typedef IMap<std::string, std::string>* (*buildMapper)(const boost::filesystem::path&);

// Set the boost logging threshold to >= info
//void init()
//{
//	boost::log::core::get()->set_filter
//	(
//		boost::log::trivial::severity >= boost::log::trivial::info
//	);
//}

int main()
{
	HINSTANCE hDLL;
	buildMapper createMap;

	const wchar_t* libName = L"..\\x64\\Debug\\MapLibrary.dll";

	hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL

	boost::filesystem::ifstream input_stream;
	std::string key = "All'sWellThatEndsWell.txt";
	std::string value;

	int success = 0;
	int line_count = 0;

	input_stream.open("C:\\Users\\thric\\Documents\\Syracuse\\CIS_687_Object_Oriented_Design\\Project2\\Project2\\MapReduce\\shakespeare\\All'sWellThatEndsWell.txt");

	
	if (hDLL != NULL) {
		createMap = (buildMapper)GetProcAddress(hDLL, "createMapper");
		IMap<std::string, std::string>* mapper;

		if (createMap != NULL) {
			mapper = createMap(boost::filesystem::path{ "C:\\Users\\thric\\Documents\\Syracuse\\CIS_687_Object_Oriented_Design\\Project2\\Project2\\MapReduce\\temp" });
			std::cout << "Loader mapper" << std::endl;

			std::cout << mapper->getOutputDirectory().string();

			int success;

			// Process all lines of the file via map
			while (std::getline(input_stream, value))
			{
				// Increment the line count and call the map object to map the key and the value
				line_count++;
				success = mapper->map(key, value);
				std::cout << success << std::endl; // Check map fails
			}

			line_count = 0;
			delete mapper;
		}
		else
			std::cout << "Did not load mapper correctly." << std::endl;
		FreeLibrary(hDLL);
	}
	else {
		std::cout << "Library load failed!" << std::endl;
	}

	std::cin.get();
	return 0;
}

/*
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <windows.h>
#include <ireduce.h>
#include <boost/filesystem.hpp>
#include <boost/container/map.hpp>

typedef IReduce<std::string, int>* (*buildReducer)(const boost::filesystem::path&);

int main()
{
	HINSTANCE hDLL;
	buildReducer createReduce;

	const wchar_t* libName = L"C:\\Users\\llaba\\source\\repos\\CSE687OOD_Project2\\x64\\Debug\\ReduceLibrary.dll";

	hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL

	boost::container::map<std::string, std::vector<int>> map_data;
	std::vector<int> one{ 1 };
	std::vector<int> two{ 1, 1 };
	map_data["the"] = two;
	map_data["quick"] = one;
	map_data["brown"] = one;
	map_data["fox"] = one;
	map_data["jumps"] = one;
	map_data["over"] = one;
	map_data["lazy"] = one;
	map_data["dog"] = one;


	if (hDLL != NULL) {
		createReduce = (buildReducer)GetProcAddress(hDLL, "createReducer");
		IReduce<std::string, int>* reducer;

		if (createReduce != NULL) {
			reducer = createReduce(boost::filesystem::path{ "C:\\Users\\llaba\\source\\repos\\CSE687OOD_Project2\\MapReduce\\temp" });
			std::cout << "Loader reducer" << std::endl;

			std::cout << reducer->getOutputDirectory().string();

			int success;
			for (auto const& pair : map_data) {
				success = reducer->reduce(pair.first, pair.second);
				std::cout << success << std::endl; // Check reduce fails
			}

			delete reducer;
		}
		else
			std::cout << "Did not load reducer correctly." << std::endl;
		FreeLibrary(hDLL);
	}
	else {
		std::cout << "Library load failed!" << std::endl;
	}

	std::cin.get();
	return 0;
}*/


// mapreduce.cpp
// Todd Hricik
// CIS 687 Object Oriented Design
// Project 1
////////////////////////////////////////////////////////////////////////////////////
// This program acts as executor which will intantiate a workflow object and invoke 
// the workflow's run member function
////////////////////////////////////////////////////////////////////////////////////
//#include <iostream>
//#include <string>
//#include <boost\log\core.hpp>
//#include <boost\log\expressions.hpp>
//#include <boost\filesystem.hpp>
//#include <boost\log\trivial.hpp>
//#include "executive.h"
//#include "workflow.h"




//int main(int argc, char* argv[])
//{
//
//
//	init();
//
//	// check that comand line input is valid
//	if (argc != 4)
//	{
//		BOOST_LOG_TRIVIAL(fatal) << "MapReduce <path of dir containging input files> <path of dir for intermediate files> <path of dir for output files>.";
//		return -1;
//	}
//
//	// declare strings to hold command line input values which should each be a path to a directory
//	std::string target_dir = argv[1];
//	std::string intermediate_dir = argv[2];
//	std::string out_dir = argv[3];
//
//	// Instantiate an executor
//	Executive* executive = new Executive(argv[1], argv[2], argv[3]);
//
//	// Use the executor to run the workflow
//	executive->run();
//
//	delete executive;
//}




