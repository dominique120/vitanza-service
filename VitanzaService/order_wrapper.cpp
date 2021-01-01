/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "order_wrapper.h"

std::string Order_wrapper::get_order(const std::string& id_or_uuid) {
	const nlohmann::json j = DynamoDB::get_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str());
	return j.dump();
}

bool Order_wrapper::update_order(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str(), request_body);
}

bool Order_wrapper::delete_order(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str());
}

bool Order_wrapper::new_order(const std::string& request_body) {
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("orders", "OrderId_uuid", id, request_body);
}

std::string Order_wrapper::get_all_orders() {
	return DynamoDB::scan_table_items_dynamo("orders");
}

std::string Order_wrapper::get_outstanding_orders() {
	std::map<std::string, std::string> conditions;
	auto pending = std::pair<std::string, std::string>("Pending", "0");
	conditions.insert(pending);
	auto paid = std::pair<std::string, std::string>("Paid", "0");
	conditions.insert(paid);

	return DynamoDB::scan_table_items_filer_dynamo("orders", conditions);
}


std::string Order_wrapper::get_order_by_client(const std::string& client_uuid) {
	return DynamoDB::query_index("orders", "ClientId_uuid", client_uuid.c_str());
}