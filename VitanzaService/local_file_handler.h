/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once
#ifndef LOCAL_FILE_HANDLER_H
#define LOCAL_FILE_HANDLER_H

#if defined(FS_LOCAL)
#include "config.h"


class Local_file {
public:
	static bool save_file(const std::string& filename, std::stringstream& image_data);
	static bool get_file(const std::string& filename, std::stringstream& stream);
	static bool delete_file(const std::string& filename);
};
#endif
#endif