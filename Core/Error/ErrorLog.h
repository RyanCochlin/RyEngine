//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document

#include <iostream>

inline void ReportAssertionFail(const char* assertType, const char* message, const char* file, int line)
{
	//TODO print to error log. For now just output to console
	std::cerr << "Assert Fail in " << file << " line " << line << ": " << assertType << " " << message;
}