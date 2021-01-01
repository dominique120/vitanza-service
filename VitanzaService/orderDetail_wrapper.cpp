/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "orderDetail_wrapper.h"

std::string OrderDetail_wrapper::get_order_detail(const std::string& id_or_uuid) {
	const nlohmann::json j = DynamoDB::get_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str());
	return j.dump();
}

bool OrderDetail_wrapper::delete_order_detail(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str());
}

bool OrderDetail_wrapper::new_order_detail(const std::string& request_body) {
	/* ---------UUID Generation -----------------*/
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("orderdetails", "OrderDetailId_uuid", id.c_str(), request_body);
}

bool OrderDetail_wrapper::update_order_detail(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str(), request_body);
}

std::string OrderDetail_wrapper::get_orderdetails_by_order(const std::string& id_or_uuid) {
	return DynamoDB::query_index("orderdetails", "OrderId_uuid", id_or_uuid.c_str());
}
