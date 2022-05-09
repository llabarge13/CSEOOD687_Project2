// workflow.h
// Todd Hricik
// CSE687 Object Oriented Design
// Project 1
// April 28, 2022
// 
// Workflow class definition
#pragma once
#include "boost\filesystem.hpp"
#include "imap.h"
#include "sorting.h"
#include "ireduce.h"

typedef IMap<std::string, std::string>* (*buildMapper)(const boost::filesystem::path&);
typedef IReduce<std::string, int>* (*buildReducer)(const boost::filesystem::path&);

class Workflow
{
	public:
		// Public member functions
		Workflow(std::string input_dir_arg, std::string inter_dir_arg, std::string output_dir_arg, std::string map_dll_path, std::string reduce_dll_path);		// constructor
		~Workflow();																			// destructor
		boost::filesystem::path getTargetDir();													// directory containing files to be fed to Map
		boost::filesystem::path getIntermediateDir();											// directory containing intermediate files from Map
		boost::filesystem::path getOutDir();													// directory containing output file(s) from reduce
		void run();																				// Runs the map sort reduce workflow

		// Public data members

	private:
		// Private member functions
		// Private data members
		boost::filesystem::path target_dir_;
		boost::filesystem::path intermediate_dir_;
		boost::filesystem::path out_dir_;

		IMap<std::string, std::string> * map_;													// Map is aggregated by Workflow
		Sorting* sorter_;																		// Sorting is aggregated by Workflow
		IReduce<std::string, int>* reduce_;														// Reduce is aggregated by Workflow

		buildMapper create_map_;
		buildReducer create_reduce_;

		HINSTANCE hDLL_map;
		HINSTANCE hDLL_reduce;

};

