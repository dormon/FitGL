#include "Loader.h"

#include <fstream>
#include <iostream>

std::string Loader::text(std::string const & fileName) {
	std::ifstream f(fileName.c_str());
	if (!f.is_open()) {
		std::string er = "File not found:" + fileName;
		throw std::exception(er.c_str());
		return "";
	}
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	f.close();
	return str;
}