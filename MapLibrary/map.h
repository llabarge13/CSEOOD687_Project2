// Map class definition
#include <string>
#include <array>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <imap.h>
#pragma once

// Performs map operation on text files
// Given data from a file, outputs a separate temporary file that 
// holds (word, 1) for each occurrence of every word.
// Example:
//	std::string text = "The quick brown fox jumps over the lazy dog.";
//	std::string file = "test.txt";
//	Map m = Map(directory);
//	int success = m.map(file, text);
class Map : public IMap<std::string, std::string>
{

public:

	Map(const boost::filesystem::path& directory);

	// Given data from a file, writes to a separate temporary file that 
	// holds (word, 1) for each occurrence of every word.
	// If the map operation failed (could not write to the file), 
	// returns -1.
	// If the map operation succeeded, returns 0.
	int map(const std::string& key, const std::string& value);

};
