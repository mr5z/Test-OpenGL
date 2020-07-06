#include <fstream>
#include <sstream>
#include <iostream>

#include "FileReader.h"

std::string FileReader::readAllFiles(const std::string& path) {
	if (!fileExist(path)) {
		std::string message("File does not exist: ");
		message += path;
		throw std::exception(message.c_str());
	}

	std::ifstream fileStream(path);
	std::stringstream stream;
	stream << fileStream.rdbuf();
	fileStream.close();
	return stream.str();
}