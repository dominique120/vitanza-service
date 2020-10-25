
#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <cstdint>
#include <string>
#include <list>
#include <sstream>
#include <nlohmann/json.hpp>
#include "db_mysql.h"

class Product {

public:
	int32_t product_id;
	std::string product_name;
	std::string product_description;
	float_t price;
	int32_t available_stock;

	std::string product_id_uuid;

	using ProductList = std::list<Product>;

	//db access

	// select all OrderDetails
	static ProductList select_products();

	//select OrderDetail by ID
	static Product select_product_by_id(const int32_t& id);
	static Product select_product_by_id(const std::string& uuid);

	//update OrderDetail
	static bool update_product(const Product& product);

	//new OrderDetail
	static bool new_product(const Product& product);

	//delete OrderDetail
	static bool delete_product(int32_t id);
	static bool delete_product(const std::string& uuid);


	/* ----------- Json methods ---------------*/
	static std::string to_json_array(const ProductList& product);

	static void from_json(const nlohmann::json& j, Product& s);
	static void to_json(nlohmann::json& j, const Product& s);
};

#endif
