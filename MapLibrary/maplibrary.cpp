// map.cpp
// Lyndsay LaBarge
// CSE687 Object Oriented Design
// Project 1
// April 28, 2022
// 
// Map class implementation
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "map.h"


Map::Map(const boost::filesystem::path& directory) : IMap<std::string, std::string>(directory)
{
}


// Given data from a file, writes to a separate temporary file that 
// holds (word, 1) for each occurrence of every word.
// If the map operation failed (could not write to the file), 
// returns -1.
// If the map operation succeeded, returns 0.
int Map::map(const std::string& key, const std::string& value)
{
	int export_success;
	boost::tokenizer<> tok(value);
	// Tokenize text
	for (boost::tokenizer<>::iterator beg = tok.begin();
		beg != tok.end();
		++beg) {
		// Normalize text (remove capitalization)
		std::string token = *beg;
		boost::algorithm::to_lower(token);
		// Export token
		export_success = exportToDisk(key, token, 1);

		// Export failed
		if (export_success != 0) {
			return -1;
		}
	}
	// Map success
	return 0;
}
