#include "Product.h"


Product::ProductList Product::selectProducts() {
	ProductList returnedTable;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products`;";

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		Product product;
		product.ProductId = result->getNumber<int32_t>("ProductId");
		product.ProductName = result->getString("ProductName").c_str();
		product.ProductDescription = result->getString("ProductDescription").c_str();
		product.Price = result->getNumber<float_t>("Price");
		product.AvailableStock = result->getNumber<int32_t>("AvailableStock");
		returnedTable.push_back(product);
	} while (result->next());
	return returnedTable;
}

Product Product::selectProductById(int32_t id) {
	Product product;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products` WHERE ProductId = ";
	query << id;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return product;
	}

	product.ProductId = result->getNumber<int32_t>("ProductId");
	product.ProductName = result->getString("ProductName").c_str();
	product.ProductDescription = result->getString("ProductDescription").c_str();
	product.Price = result->getNumber<float_t>("Price");
	product.AvailableStock = result->getNumber<int32_t>("AvailableStock");

	return product;
}

Product Product::selectProductById(std::string uuid) {
	Product product;
	std::ostringstream query;
	query << "SELECT `products`.`ProductId`, `products`.`ProductName`, `products`.`ProductDescription`, `products`.`Price`, `products`.`AvailableStock` FROM `vitanza`.`products` WHERE ProductId_uuid = ";
	query << uuid;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return product;
	}

	product.ProductId = result->getNumber<int32_t>("ProductId");
	product.ProductName = result->getString("ProductName").c_str();
	product.ProductDescription = result->getString("ProductDescription").c_str();
	product.Price = result->getNumber<float_t>("Price");
	product.AvailableStock = result->getNumber<int32_t>("AvailableStock");

	return product;
}

bool Product::updateProduct(Product product) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `vitanza`.`products` SET ";
	query << "`ProductName` = " << db.escapeString(product.ProductName) << " ,";
	query << "`ProductDescription` = " << db.escapeString(product.ProductDescription) << " ,";
	query << "`Price` = " << product.Price << " ,";
	query << "`AvailableStock` = " << product.AvailableStock;
	query << "WHERE `ProductId` = " << product.ProductId;
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Product::newProduct(Product product) {
	Database db;
	std::ostringstream query;

	query << "INSERT INTO `vitanza`.`products`(`ProductName`,`ProductDescription`,`Price`,`AvailableStock`) VALUES( ";
	query << db.escapeString(product.ProductName) << " ,";
	query << db.escapeString(product.ProductDescription) << " ,";
	query << product.Price << " ,";
	query << product.AvailableStock;
	query << " ); ";

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}

}

bool Product::deleteProduct(int32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`products` WHERE `products`.`ProductId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Product::deleteProduct(std::string uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`products` WHERE `products`.`ProductId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}


std::string Product::to_json_array(ProductList product) {
	nlohmann::json j;
	for (auto const& i : product) {
		j.push_back({
			{ "ProductId",i.ProductId},
			{ "ProductId_uuid",i.ProductId_uuid},
			{ "ProductName", i.ProductName},
			{ "ProductDescription", i.ProductDescription},
			{ "Price", i.Price},
			{ "AvailableStock", i.AvailableStock}
					});
	}

	return j.dump();
}

void Product::to_json(nlohmann::json& j, const Product& s) {
	j [ "ProductId" ] = s.ProductId;
	j [ "ProductId_uuid" ] = s.ProductId_uuid;
	j [ "ProductName" ] = s.ProductName;
	j [ "ProductDescription" ] = s.ProductDescription;
	j [ "Price" ] = s.Price;
	j [ "AvailableStock" ] = s.AvailableStock;
}

void Product::from_json(const nlohmann::json& j, Product& s) {

	try {
		s.ProductId = j.at("ProductId").get<int32_t>();
		s.ProductId_uuid = j.at("ProductId_uuid").get<std::string>();
	} catch (nlohmann::json::exception) {
		// if ProductId is empty then its a new product and this is expected behavior
	}

	try {
		s.ProductName = j.at("ProductName").get<std::string>();
		s.ProductDescription = j.at("ProductDescription").get<std::string>();
		s.Price = j.at("Price").get<float_t>();
		s.AvailableStock = j.at("AvailableStock").get<int32_t>();
	} catch (nlohmann::json::exception) {
		// these fields must be complete
		std::cout << "Product sent with invalid mandatory fields" << std::endl;
	}
}