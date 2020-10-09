
#ifndef CONFIG_H
#define CONFIG_H
#include <string>
class config {
public:
	const std::string configFileName = "config.dv";

	// write custom parser for configuration file
	// it will be simply -> key : value <- list of items
	// that will be parsed into an std::map(maybe)
	// and assigned to a class
};

#endif