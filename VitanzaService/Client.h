/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
	// members
	uint32_t client_id;
	std::string first_name;
	std::string last_names;
	std::string primary_address;
	std::string secondary_address;
	std::string primary_phone;
	std::string secondary_phone;
	std::string district;

	std::string client_id_uuid;

	using ClientList = std::list<Client>;

	/* --------------- to and from json functions -----------------*/
	static std::string to_json_array(const ClientList& cl);

	void from_json(const nlohmann::json& j, Client& s) const;
	static void to_json(nlohmann::json& j, const Client& s);
};

#endif