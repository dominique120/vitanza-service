/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "auth_wrapper.h"

bool auth_wrapper::save_new_user(const std::string& username, const std::string& password) {
	return true;
}

bool auth_wrapper::authenticate(const std::string& username, const std::string& password) {
	return true;
}
