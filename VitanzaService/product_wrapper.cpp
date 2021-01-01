/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "product_wrapper.h"

std::string Product_wrapper::get_product(const std::string& id_or_uuid) {
	const nlohmann::json j = DynamoDB::get_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str());
	return j.dump();
}

bool Product_wrapper::delete_product(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str());
}

bool Product_wrapper::new_product(const std::string& request_body) {
	/* ---------UUID Generation -----------------*/
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("products", "ProductId_uuid", id.c_str(), request_body);
}

bool Product_wrapper::update_product(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str(), request_body);
}

std::string Product_wrapper::get_all_products() {
	return DynamoDB::scan_table_items_dynamo("products");
}