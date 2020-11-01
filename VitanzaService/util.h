#ifndef UTIL_H
#define UTIL_H

extern "C" char* strptime(const char* s,
						  const char* f,
						  struct tm* tm);

time_t String2time_t(const std::string& strDateTime);

#endif