// executive.cpp
// Todd Hricik
// CSE687 Object Oriented Design
// Project 1
// April 28, 2022
// 
// Executive class implementation
#include <string>
#include "executive.h"
#include "workflow.h"


/*	Constructor takes input directory, dir for intermediate files, output directory,
	path to Map DLL and path to Reduce DLL as args
*/
Executive::Executive(std::string input_dir_arg, std::string inter_dir_arg, std::string output_dir_arg, std::string map_dll_path, std::string reduce_dll_path)
{
	workflow_ = new Workflow(input_dir_arg, inter_dir_arg, output_dir_arg, map_dll_path, reduce_dll_path);
}

// Destructor
Executive::~Executive()
{
	delete workflow_;
}

// Calls on workflow object to run its map/sort/reduce application
void Executive::run()
{
	workflow_->run();
}