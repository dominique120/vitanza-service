/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once
#ifndef AUTH_WRAPPER_H
#define AUTH_WRAPPER_H

class auth_wrapper {
public:
	static bool save_new_user(const std::string& username, const std::string& password);
	static bool authenticate(const std::string& username, const std::string& password);
};
#endif