/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CLIENT_H
#define CLIENT_H

#include "db_mysql.h"

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

#if defined(DB_MYSQL)
	/*-------------- db access -----------------------*/

	// select all clients
	static ClientList select_clients();

	//select client by ID
	static Client select_client_by_id(const uint32_t& id);
	static Client select_client_by_id(const std::string& uuid);

	//update client
	static bool update_client(const Client& client);

	//new client
	static bool new_client(const Client& client);

	//delete client
	static bool delete_client(const uint32_t& id);
	static bool delete_client(const std::string& uuid);
#endif

	/* --------------- to and from json functions -----------------*/
	static std::string to_json_array(const ClientList& cl);

	void from_json(const nlohmann::json& j, Client& s) const;
	static void to_json(nlohmann::json& j, const Client& s);
};

#endif