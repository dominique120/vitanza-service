
#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <cstdint>
#include <string>
#include <list>
#include <sstream>
#include <nlohmann\json.hpp>
#include "db_mysql.h"

class Product {

public:
	int32_t ProductId;
	std::string ProductName;
	std::string ProductDescription;
	float_t Price;
	int32_t AvailableStock;

	std::string ProductId_uuid;

	using ProductList = std::list<Product>;

	//db access

	// select all OrderDetails
	ProductList selectProducts();

	//select OrderDetail by ID
	Product selectProductById(int32_t id);
	Product selectProductById(std::string uuid);

	//update OrderDetail
	bool updateProduct(Product product);

	//new OrderDetail
	bool newProduct(Product product);

	//delete OrderDetail
	bool deleteProduct(int32_t id);
	bool deleteProduct(std::string uuid);


	/* ----------- Json methods ---------------*/
	std::string to_json_array(ProductList product);

	void from_json(const nlohmann::json& j, Product& s);
	void to_json(nlohmann::json& j, const Product& s);
};

#endif
