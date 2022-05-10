// executive.h
// Todd Hricik
// CSE687 Object Oriented Design
// Project 1
// April 28, 2022
// 
// Executive class definition
#include <string>
#include "workflow.h"
#pragma once

/*	Executive takes command line arguments into constructor
	and instantiates a Workflow object that will run the 
	map reduce application.
*/
class Executive
{
	public:
		Executive(std::string target_dir, std::string intermediate_dir, std::string out_dir, std::string map_dll_path, std::string reduce_dll_path);		
		~Executive();
		// Calls workflow to run
		void run();

	private:
		Workflow* workflow_;
};
