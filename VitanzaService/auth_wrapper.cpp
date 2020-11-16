/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "auth_wrapper.h"

bool auth_wrapper::save_new_user(const std::string& username, const std::string& password) {
	if(Auth::create_user(username, password)) {
		return true;
	} else {
		return false;
	}
}

bool auth_wrapper::authenticate(const std::string& username, const std::string& password) {
	if (Auth::validate_user(username, password)) {
		return true;
	} else {
		return false;
	}
}
