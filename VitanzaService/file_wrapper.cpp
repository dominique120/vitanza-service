/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "file_wrapper.h"

extern ConfigurationManager g_config;

bool File_wrapper::write_fs(const std::string& filename, std::stringstream& image_data) {
#if defined(FS_S3)
	if (S3::put_object_s3(filename, image_data, true)) {
		return true;
	} else {
		return false;
	}
#elif defined(FS_LOCAL)
	if (Local_file::save_file(filename, image_data)) {
		return true;
	} else {
		return false;
	}
#else
	return false;
#endif
}

bool File_wrapper::read_fs(const std::string& filename, std::stringstream& stream) {
#if defined(FS_S3)
	if (S3::get_object_s3(filename, stream)) {
		return true;
	} else {
		return false;
	}
#elif defined(FS_LOCAL)
	if (Local_file::get_file(filename, stream)) {
		return true;
	} else {
		return false;
	}
#else
	return false;
#endif
}

bool File_wrapper::delete_fs(const std::string& filename) {
#if defined(FS_S3)
	if (S3::delete_object_s3(filename)) {
		return true;
	} else {
		return false;
	}
#elif defined(FS_LOCAL)
	if (Local_file::delete_file(filename)) {
		return true;
	} else {
		return false;
	}
#else
	return false;
#endif
}
