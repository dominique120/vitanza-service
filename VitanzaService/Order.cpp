#include "Order.h"

bool Order::updateOrder(Order order) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `orders` SET `ClientId` = " << order.ClientId << " ,";
	query << "`DatePlaced` = " << order.DatePlaced << ",";
	query << "`Notes` = " << db.escapeString(order.Notes) << " ,";
	query << "`Delivered` = " << order.Delivered << " ,";
	query << "`DateDelivered` = " << order.DateDelivered << " ,";
	query << "`Paid` = " << order.Paid;
	query << "WHERE `OrderId` = " << order.OrderId;

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::newOrder(Order order) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `orders`(`ClientId`,`Notes`)VALUES( ";
	query << order.ClientFullName << " ,";
	query << db.escapeString(order.Notes) << ")";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::deleteOrder(int32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `orders`WHERE OrderId = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::deleteOrder(std::string uuid) {
	std::ostringstream query;
	query << "DELETE FROM `orders`WHERE OrderId_uuid = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

Order::OrderList Order::selectOrders() {
	OrderList returnedTable;
	std::ostringstream query;
	query << "SELECT * FROM orders;";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		Order order;
		order.ClientId = result->getNumber<uint32_t>("ClientId");
		order.OrderId = result->getNumber<uint32_t>("OrderId");
		order.DatePlaced = String2time_t((std::string)result->getString("DatePlaced"));
		order.Notes = result->getString("Notes").c_str();
		order.DateDelivered = String2time_t((std::string)result->getString("DateDelivered"));
		std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.Delivered;
		std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.Paid;
		returnedTable.push_back(order);
	} while (result->next());
	return returnedTable;
}

Order::OrderList Order::selectPendingOrders() {
	OrderList returnedTable;
	std::ostringstream query;
	query << "SELECT o.OrderId, c.ClientId, o.DatePlaced, o.Notes, CONCAT(c.FirstName, ' ', c.LastNames) AS ClientFullName, o.Paid, o.Delivered FROM orders o INNER JOIN clients c ON o.ClientId = c.ClientId WHERE `Delivered` = false and `Paid` = false";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		Order order;
		order.ClientId = result->getNumber<uint32_t>("ClientId");
		order.OrderId = result->getNumber<uint32_t>("OrderId");
		order.DatePlaced = String2time_t((std::string)result->getString("DatePlaced"));
		order.Notes = result->getString("Notes").c_str();
		order.DateDelivered = String2time_t((std::string)result->getString("DateDelivered"));
		std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.Delivered;
		std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.Paid;
		order.ClientFullName = result->getString("ClientFullName");
		returnedTable.push_back(order);
	} while (result->next());
	return returnedTable;
}

Order Order::selectOrderById(int32_t id) {
	Order order;
	std::ostringstream query;
	query << "SELECT * FROM orders WHERE OrderId = " << id;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order;
	}
	order.ClientId = result->getNumber<uint32_t>("ClientId");
	order.OrderId = result->getNumber<uint32_t>("OrderId");
	order.DatePlaced = String2time_t((std::string)result->getString("DatePlaced"));
	order.Notes = result->getString("Notes").c_str();
	order.DateDelivered = String2time_t((std::string)result->getString("DateDelivered"));
	std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.Delivered;
	std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.Paid;

	return order;
}

Order Order::selectOrderById(std::string uuid) {
	Order order;
	std::ostringstream query;
	query << "SELECT * FROM orders WHERE OrderId_uuid = " << uuid;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order;
	}
	order.ClientId = result->getNumber<uint32_t>("ClientId");
	order.OrderId = result->getNumber<uint32_t>("OrderId");
	order.DatePlaced = String2time_t((std::string)result->getString("DatePlaced"));
	order.Notes = result->getString("Notes").c_str();
	order.DateDelivered = String2time_t((std::string)result->getString("DateDelivered"));
	std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.Delivered;
	std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.Paid;

	return order;
}


std::string Order::to_json_array(OrderList order) {
	nlohmann::json j;
	for (auto const& i : order) {
		j.push_back({
			{ "OrderId",i.OrderId},
			{ "OrderId_uuid",i.OrderId_uuid},
			{ "ClientId", i.ClientId},
			{ "DatePlaced", i.DatePlaced},
			{ "Notes",i.Notes},
			{ "Delivered", i.Delivered},
			{ "Paid", i.Paid},
			{ "DateDelivered", i.DateDelivered}
					});
	}
	return j.dump();
}


void Order::to_json(nlohmann::json& j, const Order& s) {
	j [ "OrderId" ] = s.OrderId;
	j [ "ClientId" ] = s.ClientId;
	j [ "DatePlaced" ] = s.DatePlaced;
	j [ "Notes" ] = s.Notes;
	j [ "Delivered" ] = s.Delivered;
	j [ "Paid" ] = s.Paid;
	j [ "DateDelivered" ] = s.DateDelivered;
	j [ "OrderId_uuid" ] = s.OrderId_uuid;
}


void Order::from_json(const nlohmann::json& j, Order& s) {
	try {
		s.OrderId = j.at("OrderId").get<int32_t>();
		s.OrderId_uuid = j.at("OrderId_uuid").get<std::string>();
		s.DateDelivered = j.at("DateDelivered").get<time_t>();
	} catch (nlohmann::json::exception) {
		// Expected behavior
	}

	try {
		s.DatePlaced = j.at("DatePlaced").get<time_t>();
		s.ClientId = j.at("ClientId").get<int32_t>();
		s.Notes = j.at("Notes").get<std::string>();
		s.Delivered = j.at("Delivered").get<bool>();
		s.Paid = j.at("Paid").get<bool>();
	} catch (nlohmann::json::exception) {
		std::cout << "Order sent with missing mandatory data" << std::endl;
	}

}