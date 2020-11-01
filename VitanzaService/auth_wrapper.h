#pragma once

class auth_wrapper {
public:
	static bool save_new_user(const std::string& username, const std::string& password);
	static bool authenticate(const std::string& username, const std::string& password);
};
