/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "chemical_orderDetail_wrapper.h"

std::string Chemical_Order_Detail_Wrapper::get_order_detail(const std::string& id_or_uuid) {
	nlohmann::json j;
	DynamoDB::get_item_dynamo("ch_orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str(), j);
	return j.dump();
}

bool Chemical_Order_Detail_Wrapper::delete_order_detail(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("ch_orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str());
}

bool Chemical_Order_Detail_Wrapper::new_order_detail(const std::string& request_body) {
	/* ---------UUID Generation -----------------*/
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("ch_orderdetails", "OrderDetailId_uuid", id.c_str(), request_body);
}

bool Chemical_Order_Detail_Wrapper::update_order_detail(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("ch_orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str(), request_body);
}

std::string Chemical_Order_Detail_Wrapper::get_orderdetails_by_order(const std::string& id_or_uuid) {
	nlohmann::json j;
	DynamoDB::query_index("ch_orderdetails", "OrderId_uuid", id_or_uuid.c_str(), j);
	return j.dump();
}
