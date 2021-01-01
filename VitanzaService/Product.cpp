/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "Product.h"

std::string Product::to_json_array(const ProductList& product) {
	nlohmann::json j;
	for (auto const& i : product) {
		j.push_back({
			{ "ProductId",i.product_id},
			{ "ProductId_uuid",i.product_id_uuid},
			{ "ProductName", i.product_name},
			{ "ProductDescription", i.product_description},
			{ "Price", i.price},
			{ "AvailableStock", i.available_stock}
			});
	}

	return j.dump();
}

void Product::to_json(nlohmann::json& j, const Product& s) {
	j["ProductId"] = s.product_id;
	j["ProductId_uuid"] = s.product_id_uuid;
	j["ProductName"] = s.product_name;
	j["ProductDescription"] = s.product_description;
	j["Price"] = s.price;
	j["AvailableStock"] = s.available_stock;
}

void Product::from_json(const nlohmann::json& j, Product& s) {
	try {
		s.product_id = j.at("ProductId").get<int32_t>();
		s.product_id_uuid = j.at("ProductId_uuid").get<std::string>();
	}
	catch (nlohmann::json::exception) {
		// if ProductId is empty then its a new product and this is expected behavior
	}

	try {
		s.product_name = j.at("ProductName").get<std::string>();
		s.product_description = j.at("ProductDescription").get<std::string>();
		s.price = j.at("Price").get<float_t>();
		s.available_stock = j.at("AvailableStock").get<int32_t>();
	}
	catch (nlohmann::json::exception) {
		// these fields must be complete
		std::cout << "Product sent with invalid mandatory fields" << std::endl;
	}
}