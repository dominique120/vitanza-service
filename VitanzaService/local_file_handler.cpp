/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include "vtspch.h"
#include "local_file_handler.h"

#if defined(FS_LOCAL)
extern ConfigurationManager g_config;

bool Local_file::save_file(const std::string& filename, std::stringstream& image_data) {

	std::ofstream myfile(g_config.FS_LOCAL_DIR() + "/" + filename, std::ios::binary);
	if (myfile.is_open()) {
		myfile << image_data.rdbuf();
		myfile.close();
		return true;
	} else {
		std::cout << "Unable to open file:" + filename;
		return false;
	}
}

bool Local_file::get_file(const std::string& filename, std::stringstream& stream) {
	std::ifstream myfile(g_config.FS_LOCAL_DIR() + "/" + filename, std::ios::binary);
	if (myfile.is_open()) {
		stream << myfile.rdbuf();
		myfile.close();
		return true;
	} else {
		std::cout << "Unable to open file:" + filename;
		return false;
	}
}

bool Local_file::delete_file(const std::string& filename) {
	if (std::filesystem::remove(g_config.FS_LOCAL_DIR() + "/" + filename)) {
		return true;
	} else {
		return false;
	}
}

#endif