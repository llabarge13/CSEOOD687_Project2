// imap.h
// Lyndsay LaBarge, Todd Hricik
// CSE687 Object Oriented Design
// May 12, 2022
// 
// Template class IMap is a base clase that serves as an interface
// for the map library.
// 
// Abstract class.
#include <array>
#include <regex>
#include <mutex>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost\log\trivial.hpp>
#pragma once

template <typename KEYT, typename VALUET>
class IMap {

public:
	// Create a new IMap that will write map results to 
	// given directory
	IMap(const boost::filesystem::path& directory);

	// IMap concrete class object must be deleted or go out of scope
	// and be destroyed on the stack, otherwise the output
	// file will not be able to be read until the
	// object relinquishes it .
	~IMap();

	// Key = file name, value = Line of text from file. Returns 0 on success. 
	// Returns -1 on failure.
	//
	// Abstract class method, must be defined.
	virtual int map(const KEYT& key, const VALUET& value) = 0;

	// Gets the path of the most recently written to output file  
	boost::filesystem::path getOutputPath();

	// Gets the output directory path 
	boost::filesystem::path getOutputDirectory();

private:
	boost::filesystem::path output_directory_;
	boost::filesystem::path output_path_;
	boost::filesystem::ofstream* output_stream_;
	std::array<char, 2048> output_buffer_;

	// Mutex(s)
	std::mutex mutex_output_stream_;

protected:
	// Exports word, value pair to disk
	// If the export fails, returns -1. 
	// If the export succeeds, returns 0.
	// Default implmentation. Can be overriden.
	virtual int exportToDisk(const std::string& filename,
		const KEYT& key,
		int value);
};

//	IMap constructor takes boost filesystem path as argument
template<typename KEYT, typename VALUET>
inline IMap<KEYT, VALUET>::IMap(const boost::filesystem::path& directory)
{
	output_directory_ = directory;
	output_stream_ = new boost::filesystem::ofstream{};
	output_stream_->rdbuf()->pubsetbuf(&output_buffer_.front(),
		output_buffer_.size());
}

// IMap concrete class object must be deleted or go out of scope
// and be destroyed on the stack, otherwise the output
// file will not be able to be read until the
// object relinquishes it .
template<typename KEYT, typename VALUET>
inline IMap<KEYT, VALUET>::~IMap()
{
	// Flush & close the file stream
	if (output_stream_->is_open()) {
		output_stream_->close();
	}
	delete output_stream_;
}

// Getter for OutputPath data member
template<typename KEYT, typename VALUET>
inline boost::filesystem::path IMap<KEYT, VALUET>::getOutputPath()
{
	return output_path_;
}

// Getter for OutputDirectory data member
template<typename KEYT, typename VALUET>
inline boost::filesystem::path IMap<KEYT, VALUET>::getOutputDirectory()
{
	return output_directory_;
}

//	Export to key, value pair to disk in the given filename.
template<typename KEYT, typename VALUET>
inline int IMap<KEYT, VALUET>::exportToDisk(const std::string& filename, const KEYT& key, int value)
{
	/*	Figure out which temporary partition file in temporary
		directory should receive the key, value pair
	*/
	/* Set the output file to temp_partion1.txt file in output directory
		if the first letter of the key is and element of the alphabet [a,k] inclusive, 
	*/
	boost::filesystem::path filePath;
	if (std::regex_match(key, std::regex("^[abcdefghik]")))
	{
		filePath = boost::filesystem::path{ output_directory_.string() + "\\temp_partition1.txt" };
	}
	// else set the output file to temp_partition2.txt file in output directory
	else
	{
		filePath = boost::filesystem::path{ output_directory_.string() + "\\temp_partition2.txt" };
	}

	// Acquire a lock for the output_stream_ data member
	mutex_output_stream_.lock();
	
	// Open the temp file in intermediate directory corresponding to filePath
	output_stream_->open(filePath, std::ios_base::app);

	// There is a problem with the output file
	if (output_stream_->fail()) {
		BOOST_LOG_TRIVIAL(fatal) << "Fatal error in IMAP::exportToDisk";
		// Release the lock for the output_stream_ data member
//		mutex_output_stream_.unlock();
		return -1;
	}
	else {
		// Acquire a lock on the output_sream_ data member
		
		// Write to the file stream 
		*(output_stream_) << "(";
		*(output_stream_) << key;
		*(output_stream_) << ", ";
		*(output_stream_) << value;
		*(output_stream_) << ")\n";
		// Release the lock for the output_stream_ data member
		mutex_output_stream_.unlock();
		return 0;
	}


}