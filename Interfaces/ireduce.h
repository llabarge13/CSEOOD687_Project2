#include <array>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#pragma once
template <typename KEYT, typename VALUET>
class IReduce
{

public:
	IReduce(const boost::filesystem::path& directory);

	// Reduce object must be deleted or go out of scope
	// and be destroyed on the stack, otherwise the output
	// file will not be able to be read until the Reduce object
	// class relinquishes it .
	~IReduce();

	// Performs reduce operation on intermediate results.
	// Given sorted data from the intermediate file, reduces the results 
	// by aggregating the values and output result to a file.
	// e.g. (word, [1, 1]) -> (word, 2)
	// If reduce operation fails (cannot write to file), returns -1. 
	// If reduce operation succeeds, returns 0.
	virtual int reduce(const KEYT& key, const std::vector<VALUET>& values) = 0;

	// Gets the path of the output file
	boost::filesystem::path getOutputPath();

	// Gets the path of the output directory
	boost::filesystem::path getOutputDirectory();

private:
	boost::filesystem::path output_directory_;
	boost::filesystem::path output_path_;
	boost::filesystem::ofstream* output_stream_;

protected:
	// Exports word, value pair to disk
	// If the export fails, returns -1. 
	// If the export succeeds, returns 0.
	virtual int exportToDisk(const KEYT& key, VALUET value);
};

template<typename KEYT, typename VALUET>
inline IReduce<KEYT, VALUET>::IReduce(const boost::filesystem::path& directory)
{
	output_directory_ = directory;
	output_path_ = boost::filesystem::path{
		output_directory_.string() + "\\reduce.txt"
	};
	output_stream_ = new boost::filesystem::ofstream{ output_path_ };
}

template<typename KEYT, typename VALUET>
inline IReduce<KEYT, VALUET>::~IReduce()
{
	// Flush & close the file stream
	if (output_stream_->is_open()) {
		output_stream_->close();
	}
	delete output_stream_;
}

template<typename KEYT, typename VALUET>
inline boost::filesystem::path IReduce<KEYT, VALUET>::getOutputPath()
{
	return output_path_;
}

template<typename KEYT, typename VALUET>
inline boost::filesystem::path IReduce<KEYT, VALUET>::getOutputDirectory()
{
	return output_directory_;
}

template<typename KEYT, typename VALUET>
inline int IReduce<KEYT, VALUET>::exportToDisk(const KEYT& key, VALUET value)
{
	if (!output_stream_->is_open()) {
		output_stream_->open(output_path_, std::ios_base::app);
	}

	if (!output_stream_->fail()) {
		// Write result to disk 
		*(output_stream_) << "(";
		*(output_stream_) << key;
		*(output_stream_) << ", ";
		*(output_stream_) << value;
		*(output_stream_) << ")\n";
		output_stream_->close();
		return 0;
	}
	else {
		return -1;
	}
}
