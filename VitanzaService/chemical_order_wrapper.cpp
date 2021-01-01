/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "chemical_order_wrapper.h"

std::string Chemical_Order_Wrapper::get_order(const std::string& id_or_uuid) {
	const nlohmann::json j = DynamoDB::get_item_dynamo("ch_orders", "OrderId_uuid", id_or_uuid.c_str());
	return j.dump();
}

bool Chemical_Order_Wrapper::update_order(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("ch_orders", "OrderId_uuid", id_or_uuid.c_str(), request_body);
}

bool Chemical_Order_Wrapper::delete_order(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("ch_orders", "OrderId_uuid", id_or_uuid.c_str());
}

bool Chemical_Order_Wrapper::new_order(const std::string& request_body) {
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("ch_orders", "OrderId_uuid", id, request_body);
}

std::string Chemical_Order_Wrapper::get_all_orders() {
	return DynamoDB::scan_table_items_dynamo("ch_orders");
}

std::string Chemical_Order_Wrapper::get_outstanding_orders() {
	std::map<std::string, std::string> conditions;
	auto pending = std::pair<std::string, std::string>("Pending", "0");
	conditions.insert(pending);
	auto paid = std::pair<std::string, std::string>("Paid", "0");
	conditions.insert(paid);

	return DynamoDB::scan_table_items_filer_dynamo("ch_orders", conditions);
}


std::string Chemical_Order_Wrapper::get_order_by_client(const std::string& client_uuid) {
	return DynamoDB::query_index("ch_orders", "ClientId_uuid", client_uuid.c_str());
}