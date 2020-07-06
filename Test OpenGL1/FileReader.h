#ifndef FILE_READER_H
#define FILE_REDER_H

#include <string>
#include <sys/stat.h>

class FileReader {
public:
	static std::string readAllFiles(const std::string& path);

	static inline bool fileExist(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
};

#endif