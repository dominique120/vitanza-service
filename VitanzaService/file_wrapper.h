/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once
#ifndef FILE_WRAPPER_H
#define FILE_WRAPPER_H

#include "config.h"

#include "s3_file_handler.h"
#include "local_file_handler.h"

class File_wrapper {
public:
	static bool write_fs(const std::string& filename, std::stringstream& image_data);
	static bool read_fs(const std::string& filename, std::stringstream& stream);
	static bool delete_fs(const std::string& filename);
};
#endif
