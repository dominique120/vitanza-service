/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "client_wrapper.h"

std::string Client_wrapper::get_client(const std::string& id_or_uuid) {
	nlohmann::json j;
	DynamoDB::get_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str(), j);
	return j.dump();
}

bool Client_wrapper::update_client(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str(), request_body);
}

bool Client_wrapper::delete_client(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str());
}

bool Client_wrapper::new_client(const std::string& request_body) {
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("clients", "ClientId_uuid", id, request_body);
}

std::string Client_wrapper::get_all_clients() {
	nlohmann::json j;
	DynamoDB::scan_table_items_dynamo("clients", j);
	return j.dump();
}