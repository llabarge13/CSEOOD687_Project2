// workflow.cpp
// Todd Hricik
// CSE687 Object Oriented Design
// 
// April 28, 2022 - Project 1
// 
// Workflow class implementation
//
// May 12, 2022 - Project 2
//	Updated for project 2 to load map and reduce functions
//	from DLLs
#include <string>
#include <iostream>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>
#include <boost\log\trivial.hpp>
#include "workflow.h"
#include "windows.h"


// Constructor that creates boost::filesystem::path objects for the input directory, intermediate files directory and the output directory
Workflow::Workflow(std::string input_dir_arg, 
		std::string inter_dir_arg, 
		std::string output_dir_arg, 
		std::string map_dll_path, 
		std::string reduce_dll_path)
{
	BOOST_LOG_TRIVIAL(debug) << "Debug in Workflow constructor: Entering constructor.";

	// Get DLL handle for map library
	aquireMapDLL(map_dll_path);

	// Get DLL handlwe for reduce library
	aquireReduceDLL(reduce_dll_path);

	// Set the input, temp and output directories
	setInputDirectory(input_dir_arg);
	setTempDirectory(inter_dir_arg);
	setOutputDirectory(output_dir_arg);

	// Populate the two member queues input_file_queue_1_ and input_tile_queue_2_
	initializeInputFileQueue();
}

// Destructor
Workflow::~Workflow()
{
	/////////// Clean up //////////////

	FreeLibrary(hDLL_map_);
	FreeLibrary(hDLL_reduce_);
}

//// Move constructor
//Workflow::Workflow(Workflow&& rhs)
//{
//	// Copy data members from rhs to this
//	this->target_dir_ = rhs.target_dir_;
//	this->intermediate_dir_ = rhs.intermediate_dir_;
//	this->out_dir_ = rhs.out_dir_;
//	this->map_lib_path_ = rhs.map_lib_path_;
//	this->reduce_lib_path_ = rhs.reduce_lib_path_;
//	this->mutex_input_file_queue_ = rhs.mutex_input_file_queue_;
//	this->mutex_temp_file_partition_1_ = std::move(rhs.mutex_temp_file_partition_1_);
//	this->mutex_temp_file_partition_2_ = std::move(rhs.mutex_temp_file_partition_2_);
//	this->create_map_ = rhs.create_map_;
//	this->create_reduce_ = rhs.create_reduce_;
//	this->hDLL_map_ = rhs.hDLL_map_;
//	this->hDLL_reduce_ = rhs.hDLL_reduce_;
//
//	// Delete the contents from this input_file_queue_
//	while ( !(this->input_file_queue_.empty()) )
//	{
//		this->input_file_queue_.pop();
//	}
//	
//	// Move the contents of rhs.input_file_queue_ to this
//	while ( !(rhs.input_file_queue_.empty()) )
//	{
//		this->input_file_queue_.push(rhs.input_file_queue_.front());
//		rhs.input_file_queue_.pop();
//	}
//}
//
//// Move assignment operator
//Workflow& Workflow::operator=(Workflow&& rhs)
//{
//	if (this == &rhs)
//	{
//		return *this;
//	}
//	
//	// Copy data members from rhs to this
//	target_dir_ = rhs.target_dir_;
//	intermediate_dir_ = rhs.intermediate_dir_;
//	out_dir_ = rhs.out_dir_;
//	map_lib_path_ = rhs.map_lib_path_;
//	reduce_lib_path_ = rhs.reduce_lib_path_;
//	mutex_input_file_queue_ = std::move(rhs.mutex_input_file_queue_);
//	mutex_temp_file_partition_1_ = std::move(rhs.mutex_temp_file_partition_1_);
//	mutex_temp_file_partition_2_ = std::move(rhs.mutex_temp_file_partition_2_);
//	create_map_ = rhs.create_map_;
//	create_reduce_ = rhs.create_reduce_;
//	hDLL_map_ = rhs.hDLL_map_;
//	hDLL_reduce_ = rhs.hDLL_reduce_;
//
//	// Delete the contents from this input_file_queue_
//	while ( !(input_file_queue_.empty()) )
//	{
//		input_file_queue_.pop();
//	}
//
//	// Move the contents of rhs.input_file_queue_ to this
//	while ( !(rhs.input_file_queue_.empty()) )
//	{
//		this->input_file_queue_.push(rhs.input_file_queue_.front());
//		rhs.input_file_queue_.pop();
//	}
//	return (*this);
//}

// Getter for target_dir_ data member
boost::filesystem::path Workflow::getTargetDir()
{
	// This function returns the boost::filesystem::path object private data member targetDir

	return this->target_dir_;
}
// Getter for intermediate_dir_ data member
boost::filesystem::path Workflow::getIntermediateDir()
{
	// This function returns the boost::filesystem::path object private data member intermediateDir
	return this->intermediate_dir_;
}
// Getter for out_dir_ data member
boost::filesystem::path Workflow::getOutDir()
{
	// This function returns the boost::filesystem::path object private data member outDir
	return this->out_dir_;
}

// Getter for map dll path
boost::filesystem::path Workflow::getMapLibPath()
{
	return this->map_lib_path_;
}

// Getter for reduce dll path
boost::filesystem::path Workflow::getReduceLibPath()
{
	return this->reduce_lib_path_;
}

// Aquire functions in the MapLibrary DLL
void Workflow::aquireMapDLL(std::string path_to_dll)
{

	// Check map DLL is a regular file 
	if (!(boost::filesystem::is_regular_file(path_to_dll)))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Map DLL is not a regular file.";
		exit(-1);
	}
	// Else get DLL handle for map
	else
	{
		std::wstring widestr = std::wstring(path_to_dll.begin(), path_to_dll.end());
		const wchar_t* widecstr = widestr.c_str();

		// Create a handle to map DLL
		hDLL_map_ = LoadLibraryEx(widecstr, NULL, NULL);   // Handle to map DLL
		if (hDLL_map_ != NULL) {
			BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Map DLL located.";
			create_map_ = (buildMapper)GetProcAddress(hDLL_map_, "createMapper");

			// If function pointer to createMap fails to be created, log and exit
			if (create_map_ == NULL)
			{
				BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Function pointer to createMap is NULL.";
				exit(-1);
			}
			this->map_lib_path_ = boost::filesystem::path{ path_to_dll };
		}

		// Else log that handle of Map DLL failed to be created and exit
		else
		{
			BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Failed to get handle of map DLL.";
			exit(-1);
		}

	}
}

// Aquire functions in the ReduceLibrary DLL
void Workflow::aquireReduceDLL(std::string path_to_dll)
{
	// Check reduce DLL is a regular file 
	if (!(boost::filesystem::is_regular_file(path_to_dll)))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Reduce DLL is not a regular file.";
		exit(-1);
	}
	// Else attempt to get a handle for the Reduce DLL
	else
	{
		std::wstring widestr = std::wstring(path_to_dll.begin(), path_to_dll.end());
		const wchar_t* widecstr = widestr.c_str();

		hDLL_reduce_ = LoadLibraryEx(widecstr, NULL, NULL);   // Handle to Reduce DLL
		if (hDLL_reduce_ != NULL) {
			BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Reduce DLL located.";
			create_reduce_ = (buildReducer)GetProcAddress(hDLL_reduce_, "createReducer");

			// If create_reduce_ function pointer is NULL, then log and exit
			if (create_reduce_ == NULL)
			{
				BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Function pointer to create_reduce_ is NULL.";
				exit(-1);
			}
			this->reduce_lib_path_ = boost::filesystem::path{ path_to_dll };
		}
		// Else log failure to get Reduce DLL handle and exit
		else
		{
			BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: Failed to get handle of reduce DLL.";
			exit(-1);
		}
	}

}

// Validate, create directories
// Set all of the paths that will be used as data directories
void Workflow::setInputDirectory(std::string s)
{
	// Create boost path object for targetDir containing input files
	boost::filesystem::path input_dir(s);

	// If path a is a directory, then assign the path to targetDir data member
	if (boost::filesystem::is_directory(input_dir))
	{
		this->target_dir_ = s;
		BOOST_LOG_TRIVIAL(info) << "targetDir member has been set in Workflow constructor.";
	}

	// If path is not a directory then log the error, print error message
	if (!(boost::filesystem::is_directory(input_dir)))
	{
		// Path received in arg[1] of cmd line entry is not a directory error
		BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: arg[1] is not a directory";
		exit(-1);
	}
}
void Workflow::setTempDirectory(std::string s)
{
	// Create boost path object for intermediateDir containing intermediate files
	boost::filesystem::path intermediate_dir(s);

	// If path b is a directory, then assign the path to intermediateDir data member
	if (boost::filesystem::is_directory(intermediate_dir))
	{
		this->intermediate_dir_ = s;
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: intermediateDir member has been set in Workflow constructor.";
	}
	if (!(boost::filesystem::is_directory(intermediate_dir)))
	{
		// Path received in arg[2] of cmd line entry is not a directory error
		BOOST_LOG_TRIVIAL(warning) << "Warning in Workflow constructor: argv[2] is not a directory" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Creating directory at " << s << " now...";

		// Create a directory for the intermediate files to go 
		if (!(boost::filesystem::create_directory(s)))
		{
			// Directory for intermediate files failed to be created
			BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: directory for intermediate files was not created.";
			exit(-1);
		}

		// Log that the directory was created 
		else
		{
			this->intermediate_dir_ = s;
			BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Directory for intermediate files created.";
		}
	}

	// Else log that the directory was created 
	else
	{
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Directory for intermediate files created.";
	}
}
void Workflow::setOutputDirectory(std::string s)
{
	// Create boost path object for outputDir that will contain output files of workflow object
	boost::filesystem::path output_dir(s);

	// If path c is a directory, then assign the path to outDir data member
	if (boost::filesystem::is_directory(output_dir))
	{
		this->out_dir_ = s;
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: outDir member has been set in Workflow constructor.";
	}
	if (!(boost::filesystem::is_directory(output_dir)))
	{
		// Path received in arg[3] of cmd line entry is not a directory error
		BOOST_LOG_TRIVIAL(warning) << "Warning in Workflow constructor: argv[3] is not a directory" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Creating directory at " << s << " now...";
		// Create a directory for the output files to go 
		if (!(boost::filesystem::create_directory(s)))
		{
			// std::cout << "Error creating directory: " << outputDir << std::endl;
			BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow constructor: directory for output files was not created.";
			exit(-1);
		}
		// Log that the directory was 
		else
		{
			this->out_dir_ = s;
			BOOST_LOG_TRIVIAL(info) << "Info in Workflow constructor: Directory for output files created.";
		}
	}
}


// Populate the input file queues with files in the input directory
void Workflow::initializeInputFileQueue()
{
	
	int file_count = 0;
	// For each file in the target directory which contains the files to be processed
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(this->target_dir_); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_regular_file(itr->path()))
		{
			this->input_file_queue_.push(itr->path());
		}
		else
		{
			// Log that a non regular file was encountered and ignored
			BOOST_LOG_TRIVIAL(info) << "Info in Workflow::run(): Non regular file: " << itr->path() << " detected and ignored.";
		}

	}
}


// Run a workflow consisting of map, sort and reduce on all files in target directory
void Workflow::runMapper()
{
	boost::filesystem::path input_file_path;
	boost::filesystem::ifstream input_stream;
	std::string key;
	std::string value;

	// Instantiate a Map object via the IMap interface
	IMap<std::string, std::string>* mapper = create_map_(this->intermediate_dir_);

	while ( !(input_file_queue_.empty()) )
	{
		int success = 0;
		int line_count = 0;

		// get a lock on the queue
		mutex_input_file_queue_.lock();
		// copoy and pop an item from the queue
		input_file_path = input_file_queue_.front();
		input_file_queue_.pop();
		// now release the lock on the queue
		mutex_input_file_queue_.unlock();

		// Set the key to the current filename
		key = input_file_path.filename().string();

		// Open a file input stream for the text contained within the file
		mutex_input_file_.lock();
		input_stream.open(input_file_path);

		// If ifstream did not open, log and break
		if (input_stream.fail() == 1)
		{
			BOOST_LOG_TRIVIAL(fatal) << "Fatal in Workflow run(): ifstream could not be opened for " << key;
			// release if locked ???

			break;
		}

		////////////// Map /////////////////////
		// Process all lines of the file via map
		while (std::getline(input_stream, value))
		{
			// reset success flag to false
			success = 0;

			// Increment the line count and call the map object to map the key and the value
			line_count++;
			success = mapper->map(input_file_path.string(), value);

			// If the map member function of the map object does not return zero (which is a success), then log and break
			if (success != 0)
			{
				BOOST_LOG_TRIVIAL(error) << "Error in Workflow run(): Map did not successfully process entire file at line " << line_count << " of " << key;
			}
		}
			
		// Log map process has completed for the current file
		BOOST_LOG_TRIVIAL(info) << "Info in Workflow run(): Map process completed for " << key;

		// Close the current input stream
		input_stream.close();
		// release the lock on the file
		mutex_input_file_.unlock();
		// Reset the line counter to zer0
		line_count = 0;
	}
}
