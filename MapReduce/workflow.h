// workflow.h
// Todd Hricik
// CSE687 Object Oriented Design
// April 28, 2022 - Project 1
// 
//  Workflow class definition
// May 12, 2022 - Updated for project 2
//  Updated for reduce and map DLLs
#pragma once
#include <queue>
#include <mutex>
#include "boost\filesystem.hpp"
#include "imap.h"
#include "sorting.h"
#include "ireduce.h"

typedef IMap<std::string, std::string>* (*buildMapper)(const boost::filesystem::path&);
typedef IReduce<std::string, int>* (*buildReducer)(const boost::filesystem::path&);

class Workflow
{
	public:
		// Constructor
		Workflow(std::string input_dir_arg, 
			std::string inter_dir_arg, 
			std::string output_dir_arg, 
			std::string map_dll_path, 
			std::string reduce_dll_path);

		// Destructor
		~Workflow();																// destructor

		/*	Workflow contains mutexes therefore
			Workflow is not copyable or assignable
		*/
		// therefore, delete the Copy constructor
		Workflow(const Workflow& rhs) = delete;
		// and, delete the Assignment operator
		Workflow& operator=(const Workflow& rhs) = delete;

		/*	Workflow is movable and move assignable
		*/
		// Move constructor
		Workflow(Workflow&& rhs);
		// Move assignment
		Workflow& operator=(Workflow&& rhs);


		// Getters
		boost::filesystem::path getTargetDir();										// directory containing files to be fed to Map
		boost::filesystem::path getIntermediateDir();								// directory containing intermediate files from Map
		boost::filesystem::path getOutDir();										// directory containing output file(s) from reduce
		boost::filesystem::path getMapLibPath();									// path to map dll
		boost::filesystem::path getReduceLibPath();									// path to reduce dll
		
		// Core funcitonality
		// Run the map part of the original run() command from project2
		void runMapper();																	


	private:

		boost::filesystem::path target_dir_;
		boost::filesystem::path intermediate_dir_;
		boost::filesystem::path out_dir_;
		boost::filesystem::path map_lib_path_;
		boost::filesystem::path reduce_lib_path_;

		// Validators and setters
		void setInputDirectory(std::string input_dir_arg);
		void setTempDirectory(std::string temp_dir_arg);
		void setOutputDirectory(std::string output_dir_arg);

		// Aquire neccessary DLL handles for map and reduce libraryies
		void aquireMapDLL(std::string pathToMapDLL);
		void aquireReduceDLL(std::string pathToReduceDLL);

		// Populate some queues with paths to input files
		void initializeInputFileQueue();

		/* Here is a queue that will feed paths to mappers.
		*/
		std::queue<boost::filesystem::path> input_file_queue_;

		// Here is a mutex for the input_file_queue_ data member
		std::mutex mutex_input_file_queue_;
		std::mutex mutex_input_file_;
		std::mutex mutex_temp_file_partition_1_;
		std::mutex mutex_temp_file_partition_2_;

		//		Sorting* sorter_;															// Sorting is aggregated by Workflow
//		IReduce<std::string, int>* reduce_;											// Reduce is aggregated by Workflow

		// Interfaces to map and reduce librarys 
		buildMapper create_map_;
		buildReducer create_reduce_;

		// Handles to map and reduce DLLs
		HINSTANCE hDLL_map_;
		HINSTANCE hDLL_reduce_;

};

