#include "Product.h"


Product::ProductList Product::select_products() {
	ProductList returned_table;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products`;";

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		Product product;
		product.product_id = result->getNumber<int32_t>("ProductId");
		product.product_name = result->getString("ProductName");
		product.product_description = result->getString("ProductDescription");
		product.price = result->getNumber<float_t>("Price");
		product.available_stock = result->getNumber<int32_t>("AvailableStock");
		returned_table.push_back(product);
	} while (result->next());
	return returned_table;
}

Product Product::select_product_by_id(const int32_t& id) {
	Product product;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products` WHERE ProductId = ";
	query << id;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return product;
	}

	product.product_id = result->getNumber<int32_t>("ProductId");
	product.product_name = result->getString("ProductName");
	product.product_description = result->getString("ProductDescription");
	product.price = result->getNumber<float_t>("Price");
	product.available_stock = result->getNumber<int32_t>("AvailableStock");

	return product;
}

Product Product::select_product_by_id(const std::string& uuid) {
	Product product;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products` WHERE ProductId_uuid = ";
	query << uuid;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return product;
	}

	product.product_id = result->getNumber<int32_t>("ProductId");
	product.product_name = result->getString("ProductName");
	product.product_description = result->getString("ProductDescription");
	product.price = result->getNumber<float_t>("Price");
	product.available_stock = result->getNumber<int32_t>("AvailableStock");

	return product;
}

bool Product::update_product(const Product& product) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `vitanza`.`products` SET ";
	query << "`ProductName` = " << db.escapeString(product.product_name) << " ,";
	query << "`ProductDescription` = " << db.escapeString(product.product_description) << " ,";
	query << "`Price` = " << product.price << " ,";
	query << "`AvailableStock` = " << product.available_stock;
	query << "WHERE `ProductId` = " << product.product_id;
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Product::new_product(const Product& product) {
	Database db;
	std::ostringstream query;

	query << "INSERT INTO `vitanza`.`products`(`ProductName`,`ProductDescription`,`Price`,`AvailableStock`) VALUES( ";
	query << db.escapeString(product.product_name) << " ,";
	query << db.escapeString(product.product_description) << " ,";
	query << product.price << " ,";
	query << product.available_stock;
	query << " ); ";

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}

}

bool Product::delete_product(int32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`products` WHERE `products`.`ProductId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Product::delete_product(const std::string& uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`products` WHERE `products`.`ProductId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}


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
	j [ "ProductId" ] = s.product_id;
	j [ "ProductId_uuid" ] = s.product_id_uuid;
	j [ "ProductName" ] = s.product_name;
	j [ "ProductDescription" ] = s.product_description;
	j [ "Price" ] = s.price;
	j [ "AvailableStock" ] = s.available_stock;
}

void Product::from_json(const nlohmann::json& j, Product& s) {

	try {
		s.product_id = j.at("ProductId").get<int32_t>();
		s.product_id_uuid = j.at("ProductId_uuid").get<std::string>();
	} catch (nlohmann::json::exception) {
		// if ProductId is empty then its a new product and this is expected behavior
	}

	try {
		s.product_name = j.at("ProductName").get<std::string>();
		s.product_description = j.at("ProductDescription").get<std::string>();
		s.price = j.at("Price").get<float_t>();
		s.available_stock = j.at("AvailableStock").get<int32_t>();
	} catch (nlohmann::json::exception) {
		// these fields must be complete
		std::cout << "Product sent with invalid mandatory fields" << std::endl;
	}
}