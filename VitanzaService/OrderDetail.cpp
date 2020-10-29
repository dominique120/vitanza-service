#include "OrderDetail.h"
#if defined(DB_MYSQL)
OrderDetail::OrderDetailList OrderDetail::select_order_details_by_order_id(const int32_t& order_id) {
	OrderDetailList returned_table;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderId` = " << order_id;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		OrderDetail order_detail;
		order_detail.order_detail_id = result->getNumber<uint32_t>("OrderDetailId");
		order_detail.order_id = result->getNumber<uint32_t>("OrderId");
		order_detail.product = result->getString("ProductName");
		order_detail.quantity = result->getNumber<uint32_t>("Quantity");
		order_detail.price = result->getNumber<float_t>("Price");
		order_detail.override_price = result->getNumber<float_t>("OverridePrice");
		returned_table.push_back(order_detail);
	} while (result->next());
	return returned_table;
}

OrderDetail::OrderDetailList OrderDetail::select_order_details_by_order_id(const std::string& uuid) {
	OrderDetailList returned_table;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderId_uuid` = " << uuid;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		OrderDetail order_detail;
		order_detail.order_detail_id = result->getNumber<uint32_t>("OrderDetailId");
		order_detail.order_id = result->getNumber<uint32_t>("OrderId");
		order_detail.product = result->getString("ProductName");
		order_detail.quantity = result->getNumber<uint32_t>("Quantity");
		order_detail.price = result->getNumber<float_t>("Price");
		order_detail.override_price = result->getNumber<float_t>("OverridePrice");
		returned_table.push_back(order_detail);
	} while (result->next());
	return returned_table;
}

OrderDetail OrderDetail::select_order_detail_by_id(const int32_t& id) {
	OrderDetail order_detail;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderDetailId` = " << id;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order_detail;
	}

	order_detail.order_detail_id = result->getNumber<uint32_t>("OrderDetailId");
	order_detail.order_id = result->getNumber<uint32_t>("OrderId");
	order_detail.product = result->getString("ProductName");
	order_detail.quantity = result->getNumber<uint32_t>("Quantity");
	order_detail.price = result->getNumber<float_t>("Price");
	order_detail.override_price = result->getNumber<float_t>("OverridePrice");

	return order_detail;
}

OrderDetail OrderDetail::select_order_detail_by_id(const std::string& uuid) {
	OrderDetail order_detail;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderDetailId_uuid` = " << uuid;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order_detail;
	}

	order_detail.order_detail_id = result->getNumber<uint32_t>("OrderDetailId");
	order_detail.order_id = result->getNumber<uint32_t>("OrderId");
	order_detail.product = result->getString("ProductName");
	order_detail.quantity = result->getNumber<uint32_t>("Quantity");
	order_detail.price = result->getNumber<float_t>("Price");
	order_detail.override_price = result->getNumber<float_t>("OverridePrice");

	return order_detail;
}

bool OrderDetail::update_order_detail(const OrderDetail& order_detail) {
	Database db;
	std::ostringstream query;

	query << "SELECT * FROM vitanza.orderdetails;UPDATE `vitanza`.`orderdetails` SET ";
	query << "`Quantity` = " << order_detail.quantity << " ,";
	query << "`OverridePrice` = " << order_detail.override_price;
	query << "WHERE `OrderDetailId` = " << order_detail.order_detail_id << "; ";

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}

}

bool OrderDetail::new_order_detail(const OrderDetail& order_detail) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `vitanza`.`orderdetails`(`OrderId`,`Product`,`Quantity`,`OverridePrice`)";
	query << "VALUES(" << order_detail.order_id << ",(";
	query << "SELECT `ProductId` FROM `products`WHERE ProductName = " << db.escapeString(order_detail.product) << " ),";
	query << order_detail.quantity << ",";
	query << order_detail.override_price << " );";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::delete_order_detail_by_order_id(const int32_t& id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::delete_order_detail_by_order_id(const std::string& uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::delete_order_detail(const int32_t& id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderDetailId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::delete_order_detail(const std::string& uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderDetailId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}
#endif

std::string OrderDetail::to_json_array(const OrderDetailList& order_detail) {

	nlohmann::json j;

	for (auto const& i : order_detail) {
		j.push_back({
			{ "OrderDetailId",i.order_detail_id},
			{ "OrderDetailId_uuid",i.order_detail_id_uuid},
			{ "OrderId", i.order_id},
			{ "OrderId_uuid", i.order_id_uuid},
			{ "Product", i.product},
			{ "Quantity", i.quantity},
			{ "OverridePrice", i.override_price}
					});
	}

	return j.dump();
}

void OrderDetail::to_json(nlohmann::json& j, const OrderDetail& s) {
	j [ "OrderDetailId" ] = s.order_detail_id;
	j [ "OrderDetailId_uuid" ] = s.order_detail_id_uuid;
	j [ "OrderId" ] = s.order_id;
	j [ "OrderId_uuid" ] = s.order_id_uuid;
	j [ "Product" ] = s.product;
	j [ "Quantity" ] = s.quantity;
	j [ "OverridePrice" ] = s.override_price;

}

void OrderDetail::from_json(const nlohmann::json& j, OrderDetail& s) {

	try {
		s.order_detail_id = j.at("OrderDetailId").get<int32_t>();
		s.order_detail_id_uuid = j.at("OrderDetailId_uuid").get<std::string>();
	} catch (nlohmann::json::exception&) {
		// Expected behavior
	}
	try {
		s.override_price = j.at("OverridePrice").get<float_t>();
		s.order_id = j.at("OrderId").get<int32_t>();
		s.quantity = j.at("Quantity").get<int32_t>();
		s.product = j.at("ProductName").get<std::string>();
		s.order_id_uuid = j.at("OrderId_uuid").get<std::string>();
	} catch (nlohmann::json::exception&) {
		std::cout << "Order Detail sent with invalid mandatory fields" << std::endl;
	}

}