#include "vtspch.h"
#include "Auth.h"

std::string Auth::hash_password(const std::string& plain_password, const std::string& salt) {
	return "";
}

std::string Auth::generate_token(const std::string& username, const std::string& password) {
	return "";
}

bool Auth::validate_token(const std::string& token) {
	return true;
}
