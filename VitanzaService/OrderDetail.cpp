#include "OrderDetail.h"

OrderDetail::OrderDetailList OrderDetail::selectOrderDetailsByOrderId(int32_t orderId) {
	OrderDetailList returnedTable;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderId` = " << orderId;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		OrderDetail orderDetail;
		orderDetail.OrderDetailId = result->getNumber<uint32_t>("OrderDetailId");
		orderDetail.OrderId = result->getNumber<uint32_t>("OrderId");
		orderDetail.Product = result->getString("ProductName");
		orderDetail.Quantity = result->getNumber<uint32_t>("Quantity");
		orderDetail.Price = result->getNumber<float_t>("Price");
		orderDetail.OverridePrice = result->getNumber<float_t>("OverridePrice");
		returnedTable.push_back(orderDetail);
	} while (result->next());
	return returnedTable;
}

OrderDetail::OrderDetailList OrderDetail::selectOrderDetailsByOrderId(std::string uuid) {
	OrderDetailList returnedTable;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderId_uuid` = " << uuid;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		OrderDetail orderDetail;
		orderDetail.OrderDetailId = result->getNumber<uint32_t>("OrderDetailId");
		orderDetail.OrderId = result->getNumber<uint32_t>("OrderId");
		orderDetail.Product = result->getString("ProductName");
		orderDetail.Quantity = result->getNumber<uint32_t>("Quantity");
		orderDetail.Price = result->getNumber<float_t>("Price");
		orderDetail.OverridePrice = result->getNumber<float_t>("OverridePrice");
		returnedTable.push_back(orderDetail);
	} while (result->next());
	return returnedTable;
}

OrderDetail OrderDetail::selectOrderDetailById(int32_t id) {
	OrderDetail orderDetail;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderDetailId` = " << id;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return orderDetail;
	}

	orderDetail.OrderDetailId = result->getNumber<uint32_t>("OrderDetailId");
	orderDetail.OrderId = result->getNumber<uint32_t>("OrderId");
	orderDetail.Product = result->getString("ProductName");
	orderDetail.Quantity = result->getNumber<uint32_t>("Quantity");
	orderDetail.Price = result->getNumber<float_t>("Price");
	orderDetail.OverridePrice = result->getNumber<float_t>("OverridePrice");

	return orderDetail;
}

OrderDetail OrderDetail::selectOrderDetailById(std::string uuid) {
	OrderDetail orderDetail;
	std::ostringstream query;
	query << "SELECT od.`OrderDetailId`,od.`OrderId`,p.`ProductName`,od.`Quantity`,od.`OverridePrice`, ";
	query << "p.`Price`FROM `vitanza`.`orderdetails` od INNER JOIN products P ON od.`Product`= p.`ProductId` ";
	query << "WHERE `OrderDetailId_uuid` = " << uuid;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return orderDetail;
	}

	orderDetail.OrderDetailId = result->getNumber<uint32_t>("OrderDetailId");
	orderDetail.OrderId = result->getNumber<uint32_t>("OrderId");
	orderDetail.Product = result->getString("ProductName");
	orderDetail.Quantity = result->getNumber<uint32_t>("Quantity");
	orderDetail.Price = result->getNumber<float_t>("Price");
	orderDetail.OverridePrice = result->getNumber<float_t>("OverridePrice");

	return orderDetail;
}

bool OrderDetail::updateOrderDetail(OrderDetail orderDetail) {
	Database db;
	std::ostringstream query;

	query << "SELECT * FROM vitanza.orderdetails;UPDATE `vitanza`.`orderdetails` SET ";
	query << "`Quantity` = " << orderDetail.Quantity << " ,";
	query << "`OverridePrice` = " << orderDetail.OverridePrice;
	query << "WHERE `OrderDetailId` = " << orderDetail.OrderDetailId << "; ";

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}

}

bool OrderDetail::newOrderDetail(OrderDetail orderDetail) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `vitanza`.`orderdetails`(`OrderId`,`Product`,`Quantity`,`OverridePrice`)";
	query << "VALUES(" << orderDetail.OrderId << ",(";
	query << "SELECT `ProductId` FROM `products`WHERE ProductName = " << db.escapeString(orderDetail.Product) << " ),";
	query << orderDetail.Quantity << ",";
	query << orderDetail.OverridePrice << " );";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::deleteOrderDetailByOrderId(int32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::deleteOrderDetailByOrderId(std::string uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::deleteOrderDetail(int32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderDetailId` = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool OrderDetail::deleteOrderDetail(std::string uuid) {
	std::ostringstream query;
	query << "DELETE FROM `vitanza`.`orderdetails` WHERE `OrderDetailId_uuid` = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}


std::string OrderDetail::to_json_array(OrderDetailList orderDetail) {

	nlohmann::json j;

	for (auto const& i : orderDetail) {
		j.push_back({
			{ "OrderDetailId",i.OrderDetailId},
			{ "OrderDetailId_uuid",i.OrderDetailId_uuid},
			{ "OrderId", i.OrderId},
			{ "OrderId_uuid", i.OrderId_uuid},
			{ "Product", i.Product},
			{ "Quantity", i.Quantity},
			{ "OverridePrice", i.OverridePrice}
					});
	}

	return j.dump();
}

void OrderDetail::to_json(nlohmann::json& j, const OrderDetail& s) {
	j [ "OrderDetailId" ] = s.OrderDetailId;
	j [ "OrderDetailId_uuid" ] = s.OrderDetailId_uuid;
	j [ "OrderId" ] = s.OrderId;
	j [ "OrderId_uuid" ] = s.OrderId_uuid;
	j [ "Product" ] = s.Product;
	j [ "Quantity" ] = s.Quantity;
	j [ "OverridePrice" ] = s.OverridePrice;

}

void OrderDetail::from_json(const nlohmann::json& j, OrderDetail& s) {

	try {
		s.OrderDetailId = j.at("OrderDetailId").get<int32_t>();
		s.OrderDetailId_uuid = j.at("OrderDetailId_uuid").get<std::string>();
	} catch (nlohmann::json::exception) {
		// Expected behavior
	}
	try {
		s.OverridePrice = j.at("OverridePrice").get<float_t>();
		s.OrderId = j.at("OrderId").get<int32_t>();
		s.Quantity = j.at("Quantity").get<int32_t>();
		s.Product = j.at("ProductName").get<std::string>();
		s.OrderId_uuid = j.at("OrderId_uuid").get<std::string>();
	} catch (nlohmann::json::exception) {
		std::cout << "Order Detail sent with invalid mandatory fields" << std::endl;
	}

}