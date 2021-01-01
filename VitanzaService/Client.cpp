/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "Client.h"

std::string Client::to_json_array(const ClientList& cl) {
	nlohmann::json j;

	for (auto const& i : cl) {
		j.push_back({
			{ "ClientId",i.client_id},
			{ "ClientId_uuid",i.client_id_uuid},
			{ "FirstName", i.first_name},
			{ "LastNames",i.last_names},
			{ "PrimaryAddress",i.primary_address},
			{ "SecondaryAddress", i.secondary_address},
			{ "PrimaryPhone", i.primary_phone},
			{ "SecondaryPhone",i.secondary_phone},
			{ "District", i.district}
			});
	}
	return j.dump();
}

void Client::to_json(nlohmann::json& j, const Client& s) {
	j["ClientId"] = s.client_id;
	j["FirstName"] = s.first_name;
	j["LastNames"] = s.last_names;
	j["PrimaryAddress"] = s.primary_address;
	j["SecondaryAddress"] = s.secondary_address;
	j["PrimaryPhone"] = s.primary_phone;
	j["SecondaryAddress"] = s.secondary_phone;
	j["District"] = s.district;
	j["ClientId_uuid"] = s.client_id_uuid;
}

void Client::from_json(const nlohmann::json& j, Client& s) const {
	try {
		s.client_id = std::stoi((j.at("ClientId").get<std::string>()));
		s.client_id_uuid = j.at("ClientId_uuid").get<std::string>();
	}
	catch (nlohmann::json::exception&) {
		// Expected Behavior
	}

	try {
		s.first_name = j.at("FirstName").get<std::string>();
		s.last_names = j.at("LastNames").get<std::string>();
		s.primary_address = j.at("PrimaryAddress").get<std::string>();
		s.secondary_address = j.at("SecondaryAddress").get<std::string>();
		s.primary_phone = j.at("PrimaryPhone").get<std::string>();
		s.secondary_phone = j.at("SecondaryPhone").get<std::string>();
		s.district = j.at("District").get<std::string>();
	}
	catch (nlohmann::json::exception&) {
		std::cout << "Client sent with missing mandatory data" << std::endl;
	}
}