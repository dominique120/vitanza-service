/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "config.h"

bool ConfigurationManager::load(int argc, char* argv[]) {

	// Load data from config file
	const std::string name("config.json");
	std::ifstream f(name.c_str());
	if (!f.good()) {
		std::cout << "[Error - ConfigManager::load] - Configuration file not found. Exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
	try {
		const auto j = nlohmann::json::parse(f, nullptr, true, true);
		const std::map<std::string, std::string> mp = j;
		this->configuration_table = mp;
	}
	catch (nlohmann::json::exception& e) {
		std::cout << "[Error - ConfigManager::load] - Failed to parse Configuration file: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	f.close();

	// If the program is started with two commandline args being "ip" and "port"
	// then parse them and override those that were loaded in the config file
	if (argc == 1) {
		return true;
	}

	if (argc == 3) {
		std::vector<std::string> all_args;

		try {
			all_args.assign(argv + 1, argv + argc);

			this->configuration_table["SERVER_IP"] = all_args.at(0);
			this->configuration_table["SERVER_PORT"] = all_args.at(1);
		}
		catch (std::exception& e) {
			std::cout << e.what();
		}
	}
	else {
		std::cout << "[Error - ConfigManager::load] - proper usage is -> ./vts \"ip\" \"port\" or no args. Ignoring input..." << std::endl;
	}

	return true;
}

bool ConfigurationManager::reload(const std::string& new_configuration) {
	try {
		const auto j = nlohmann::json::parse(new_configuration, nullptr, true, true);
		const std::map<std::string, std::string> mp = j;
		this->configuration_table = mp;
		return true;
	}
	catch (nlohmann::json::exception& e) {
		std::cout << "[Error - ConfigManager::load] - Malformed configuration request: " << e.what() << std::endl;
		return false;
	}
}

std::string ConfigurationManager::AWS_ACCESS_KEY()
{
	return this->configuration_table.at("AWS_ACCESS_KEY");
}

std::string ConfigurationManager::AWS_SECRET_KEY()
{
	return this->configuration_table.at("AWS_SECRET_KEY");
}

std::string ConfigurationManager::AWS_SESSION_TOKEN()
{
	return this->configuration_table.at("AWS_SESSION_TOKEN");
}

bool ConfigurationManager::AWS_USE_SESSION_TOKEN()
{
	bool ret;
	std::istringstream(this->configuration_table.at("AWS_USE_SESSION_TOKEN")) >> std::boolalpha >> ret;
	return ret;
}

std::string ConfigurationManager::AWS_REGION()
{
	return this->configuration_table.at("AWS_REGION");
}

std::string ConfigurationManager::AWS_DYNAMODB_ENDPOINT()
{
	return this->configuration_table.at("AWS_DYNAMODB_ENDPOINT");
}

std::string ConfigurationManager::AWS_S3_BUCKET_NAME()
{
	return this->configuration_table.at("AWS_S3_BUCKET_NAME");
}

int ConfigurationManager::SERVER_PORT()
{
	return std::stoi(this->configuration_table.at("SERVER_PORT"));
}

std::string ConfigurationManager::SERVER_IP()
{
	return this->configuration_table.at("SERVER_IP");
}

std::string ConfigurationManager::API_BASE_URL()
{
	return this->configuration_table.at("API_BASE_URL");
}

std::string ConfigurationManager::SHA512_SECRET()
{
	return this->configuration_table.at("SHA512_SECRET");
}

std::string ConfigurationManager::JWT_ISSUER()
{
	return this->configuration_table.at("JWT_ISSUER");
}

std::string ConfigurationManager::FS_LOCAL_DIR()
{
	return this->configuration_table.at("FS_LOCAL_DIR");
}

bool ConfigurationManager::LOG_TO_CONSOLE()
{
	bool ret;
	std::istringstream(this->configuration_table.at("LOG_TO_CONSOLE")) >> std::boolalpha >> ret;
	return ret;
}

bool ConfigurationManager::LOG_TO_FILE()
{
	bool ret;
	std::istringstream(this->configuration_table.at("LOG_TO_FILE")) >> std::boolalpha >> ret;
	return ret;
}

std::string ConfigurationManager::LOG_FILE()
{
	return this->configuration_table.at("LOG_FILE");
}

std::string ConfigurationManager::CORS_ORIGIN()
{
	return this->configuration_table.at("CORS_ORIGIN");
}
