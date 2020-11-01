#ifndef CONFIG_H
#define CONFIG_H

class ConfigurationManager {
public:

	bool load();

	std::string operator [] (const std::string& config_name) {
		return this->configuration_table.at(config_name);
	}

private:
	std::map<std::string, std::string> configuration_table;
};

#endif