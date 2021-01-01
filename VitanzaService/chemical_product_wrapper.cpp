/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "chemical_product_wrapper.h"

std::string Chemical_Product_Wrapper::get_product(const std::string& id_or_uuid) {
	const nlohmann::json j = DynamoDB::get_item_dynamo("ch_products", "ProductId_uuid", id_or_uuid.c_str());
	return j.dump();
}

bool Chemical_Product_Wrapper::delete_product(const std::string& id_or_uuid) {
	return DynamoDB::delete_item_dynamo("ch_products", "ProductId_uuid", id_or_uuid.c_str());
}

bool Chemical_Product_Wrapper::new_product(const std::string& request_body) {
	/* ---------UUID Generation -----------------*/
	Aws::String id = Aws::Utils::UUID::RandomUUID();
	return DynamoDB::new_item_dynamo("ch_products", "ProductId_uuid", id.c_str(), request_body);
}

bool Chemical_Product_Wrapper::update_product(const std::string& id_or_uuid, const std::string& request_body) {
	return DynamoDB::update_item_dynamo("ch_products", "ProductId_uuid", id_or_uuid.c_str(), request_body);
}

std::string Chemical_Product_Wrapper::get_all_products() {
	return DynamoDB::scan_table_items_dynamo("ch_products");
}