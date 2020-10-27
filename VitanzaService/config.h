
#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "nlohmann/json.hpp"

class ConfigurationManager {
public:

	bool load();

	std::string operator [] (const std::string& config_name) {
		return this->configuration_table.at(config_name);
	}

private:
	std::map<std::string, std::string> configuration_table;
		
	// Currently not used, just used to list possible configurations
	// Maybe map the json config file to this enum?
	/*
	enum config_options {
		MYSQL_PORT,
		MYSQL_HOST,
		MYSQL_DATABASE,
		MYSQL_USER,
		MYSQL_PASSWORD,
		MYSQL_PROTOCOL,

		AWS_ACCESS_KEY,
		AWS_SECRET_KEY,
		AWS_REGION,

		SERVER_PORT,
		SERVER_IP,
		SERVER_INIT_THREAD_POOL		
	};
	*/
};

#endif