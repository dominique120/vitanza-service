/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef PRODUCT_H
#define PRODUCT_H

class Product {
public:
	int32_t product_id;
	std::string product_name;
	std::string product_description;
	float_t price;
	int32_t available_stock;

	std::string product_id_uuid;

	using ProductList = std::list<Product>;

	/* ----------- Json methods ---------------*/
	static std::string to_json_array(const ProductList& product);

	static void from_json(const nlohmann::json& j, Product& s);
	static void to_json(nlohmann::json& j, const Product& s);
};

#endif
