/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CONFIG_H
#define CONFIG_H

class ConfigurationManager {
public:
	std::string AWS_ACCESS_KEY();
	std::string AWS_SECRET_KEY();
	std::string AWS_SESSION_TOKEN();
	bool AWS_USE_SESSION_TOKEN();
	std::string AWS_REGION();
	std::string AWS_DYNAMODB_ENDPOINT();
	std::string AWS_S3_BUCKET_NAME();
	int SERVER_PORT();
	std::string SERVER_IP();
	std::string API_BASE_URL();
	std::string API_BASE_URL_V2();
	std::string SHA512_SECRET();
	std::string JWT_ISSUER();
	std::string FS_LOCAL_DIR();
	bool LOG_TO_CONSOLE();
	bool LOG_TO_FILE();
	std::string LOG_FILE();
	std::string CORS_ORIGIN();

public:
	bool load(int argc, char* argv[]);
	bool reload(const std::string& new_configuration);

private:
	std::map<std::string, std::string> configuration_table;
	std::string operator [] (const std::string& config_name) {
		return this->configuration_table.at(config_name);
	}
};

#endif