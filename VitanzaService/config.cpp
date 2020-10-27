#include "config.h"


bool ConfigurationManager::load() {

	std::string name("config.json");

	std::ifstream f(name.c_str());
	if(!f.good()) {
		std::cout << "[Error - ConfigManager::load] - Configuration file not found. Exiting..."  << std::endl;
		exit(EXIT_FAILURE);
	}
	try {
		auto j = nlohmann::json::parse(f, nullptr, true, true);
		std::map<std::string, std::string> mp = j;
		this->configuration_table = mp;
	} catch (nlohmann::json::exception& e) {
		std::cout << "[Error - ConfigManager::load] - Failed to parse Configuration file: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	f.close();
	
	return true;
}

