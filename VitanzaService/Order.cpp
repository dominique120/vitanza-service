#include "vtspch.h"
#include "Order.h"
#if defined(DB_MYSQL)
bool Order::update_order(const Order& order) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `orders` SET `ClientId` = " << order.client_id << " ,";
	query << "`DatePlaced` = " << order.date_placed << ",";
	query << "`Notes` = " << db.escapeString(order.notes) << " ,";
	query << "`Delivered` = " << order.delivered << " ,";
	query << "`DateDelivered` = " << order.date_delivered << " ,";
	query << "`Paid` = " << order.paid;
	query << "WHERE `OrderId` = " << order.order_id;

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::new_order(const Order& order) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `orders`(`ClientId`,`Notes`)VALUES( ";
	query << order.ClientFullName << " ,";
	query << db.escapeString(order.notes) << ")";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::delete_order(const int32_t& id) {
	std::ostringstream query;
	query << "DELETE FROM `orders`WHERE OrderId = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Order::delete_order(const std::string& uuid) {
	std::ostringstream query;
	query << "DELETE FROM `orders`WHERE OrderId_uuid = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

Order::OrderList Order::select_orders() {
	OrderList returned_table;
	std::ostringstream query;
	query << "SELECT * FROM orders;";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		Order order;
		order.client_id = result->getNumber<uint32_t>("ClientId");
		order.order_id = result->getNumber<uint32_t>("OrderId");
		order.date_placed = String2time_t(static_cast<std::string>(result->getString("DatePlaced")));
		order.notes = result->getString("Notes");
		order.date_delivered = String2time_t(static_cast<std::string>(result->getString("DateDelivered")));
		std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.delivered;
		std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.paid;
		returned_table.push_back(order);
	} while (result->next());
	return returned_table;
}

Order::OrderList Order::select_pending_orders() {
	OrderList returned_table;
	std::ostringstream query;
	query << "SELECT o.OrderId, c.ClientId, o.DatePlaced, o.Notes, CONCAT(c.FirstName, ' ', c.LastNames) AS ClientFullName, o.Paid, o.Delivered FROM orders o INNER JOIN clients c ON o.ClientId = c.ClientId WHERE `Delivered` = false and `Paid` = false";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		Order order;
		order.client_id = result->getNumber<uint32_t>("ClientId");
		order.order_id = result->getNumber<uint32_t>("OrderId");
		order.date_placed = String2time_t(static_cast<std::string>(result->getString("DatePlaced")));
		order.notes = result->getString("Notes");
		order.date_delivered = String2time_t(static_cast<std::string>(result->getString("DateDelivered")));
		std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.delivered;
		std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.paid;
		order.ClientFullName = result->getString("ClientFullName");
		returned_table.push_back(order);
	} while (result->next());
	return returned_table;
}

Order Order::select_order_by_id(const int32_t& id) {
	Order order;
	std::ostringstream query;
	query << "SELECT * FROM orders WHERE OrderId = " << id;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order;
	}
	order.client_id = result->getNumber<uint32_t>("ClientId");
	order.order_id = result->getNumber<uint32_t>("OrderId");
	order.date_placed = String2time_t(static_cast<std::string>(result->getString("DatePlaced")));
	order.notes = result->getString("Notes");
	order.date_delivered = String2time_t(static_cast<std::string>(result->getString("DateDelivered")));
	std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.delivered;
	std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.paid;

	return order;
}

Order Order::select_order_by_id(const std::string& uuid) {
	Order order;
	std::ostringstream query;
	query << "SELECT * FROM orders WHERE OrderId_uuid = " << uuid;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return order;
	}
	order.client_id = result->getNumber<uint32_t>("ClientId");
	order.order_id = result->getNumber<uint32_t>("OrderId");
	order.date_placed = String2time_t(static_cast<std::string>(result->getString("DatePlaced")));
	order.notes = result->getString("Notes");
	order.date_delivered = String2time_t(static_cast<std::string>(result->getString("DateDelivered")));
	std::istringstream(result->getString("Delivered")) >> std::boolalpha >> order.delivered;
	std::istringstream(result->getString("Paid")) >> std::boolalpha >> order.paid;

	return order;
}
#endif

std::string Order::to_json_array(const OrderList& order) {
	nlohmann::json j;
	for (auto const& i : order) {
		j.push_back({
			{ "OrderId",i.order_id},
			{ "OrderId_uuid",i.order_id_uuid},
			{ "ClientId", i.client_id},
			{ "DatePlaced", i.date_placed},
			{ "Notes",i.notes},
			{ "Delivered", i.delivered},
			{ "Paid", i.paid},
			{ "DateDelivered", i.date_delivered}
					});
	}
	return j.dump();
}

void Order::to_json(nlohmann::json& j, const Order& s) {
	j [ "OrderId" ] = s.order_id;
	j [ "ClientId" ] = s.client_id;
	j [ "DatePlaced" ] = s.date_placed;
	j [ "Notes" ] = s.notes;
	j [ "Delivered" ] = s.delivered;
	j [ "Paid" ] = s.paid;
	j [ "DateDelivered" ] = s.date_delivered;
	j [ "OrderId_uuid" ] = s.order_id_uuid;
}

void Order::from_json(const nlohmann::json& j, Order& s) {
	try {
		s.order_id = j.at("OrderId").get<int32_t>();
		s.order_id_uuid = j.at("OrderId_uuid").get<std::string>();
		s.date_delivered = j.at("DateDelivered").get<time_t>();
	} catch (nlohmann::json::exception&) {
		// Expected behavior
	}

	try {
		s.date_placed = j.at("DatePlaced").get<time_t>();
		s.client_id = j.at("ClientId").get<int32_t>();
		s.notes = j.at("Notes").get<std::string>();
		s.delivered = j.at("Delivered").get<bool>();
		s.paid = j.at("Paid").get<bool>();
	} catch (nlohmann::json::exception&) {
		std::cout << "Order sent with missing mandatory data" << std::endl;
	}
}