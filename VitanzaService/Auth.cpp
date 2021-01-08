/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "Auth.h"

extern ConfigurationManager g_config;

std::string Auth::hash_password(const std::string& plain_password, const std::string& salt) {
	const std::string pwd_with_salt = plain_password + salt;
	unsigned char hash[SHA512_DIGEST_LENGTH];
	SHA512_CTX sha512;
	SHA512_Init(&sha512);
	SHA512_Update(&sha512, pwd_with_salt.c_str(), pwd_with_salt.size());
	SHA512_Final(hash, &sha512);
	std::stringstream ss;
	for (const unsigned char i : hash) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
	}
	return ss.str();
}

std::string Auth::generate_token(const std::string& username, const std::string& password) {
	const jwt::claim user(username);
	const jwt::claim pwd(password);

	auto token = jwt::create()
		.set_issuer(g_config.JWT_ISSUER())
		.set_type("JWS")
		.set_payload_claim("username", user)
		.set_payload_claim("password", pwd)
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 36000 })
		.sign(jwt::algorithm::hs512{ g_config.SHA512_SECRET() });

	return token;
}

bool Auth::validate_token(const std::string& token_header) {
	try {
		std::string token = token_header;

		// Remove the "Bearer " section of the header
		token.erase(0, 7);

		const auto decoded = jwt::decode(token);

		const nlohmann::json jwt_payload = nlohmann::json::parse(decoded.get_payload());

		const jwt::claim user(jwt_payload.at("username").get<std::string>());
		const jwt::claim pwd(jwt_payload.at("password").get<std::string>());

		const auto verifier = jwt::verify()
			.allow_algorithm(jwt::algorithm::hs512{ g_config.SHA512_SECRET() })
			.with_issuer(g_config.JWT_ISSUER())
			.with_claim("username", user)
			.with_claim("password", pwd);

		verifier.verify(decoded);
		return true;

	}
	catch (jwt::token_verification_exception&) {
		return false;
	}
	catch (nlohmann::json::exception&) {
		return false;
	}
	catch (std::invalid_argument&) {
		return false;
	}
	catch (std::runtime_error&) {
		return false;
	}
}

bool Auth::create_user(const std::string& usr, const std::string& pwd) {
	const std::string hashed_pwd = Auth::hash_password(pwd, usr);

	Aws::String id = Aws::Utils::UUID::RandomUUID();

	nlohmann::json j;
	j["username"] = usr;
	j["password"] = hashed_pwd;

	// Move Dynamo implementation to its own file
	if (DynamoDB::new_item_dynamo("users", "user_id", id, j.dump())) {
		return true;
	}
	else {
		return false;
	}
}

bool Auth::validate_user(const std::string& usr, const std::string& pwd) {
	const std::string hashed_pwd = Auth::hash_password(pwd, usr);
	nlohmann::json j;

	// Move Dynamo implementation to its own file

	DynamoDB::query_index("users", "username", usr.c_str(), j);

	std::map<std::string, std::string> user = j[0];

	bool valid_usr = false;
	bool valid_pwd = false;
	try {
		if (user.at("username") == usr) {
			valid_usr = true;
		}

		if (user.at("password") == hashed_pwd) {
			valid_pwd = true;
		}

		if (valid_usr && valid_pwd) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (const nlohmann::json::exception& ex) {
		std::cout << "user validation failed with: " << ex.what() << "\n";
		return false;		
	}
}
