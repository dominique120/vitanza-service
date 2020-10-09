#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <cstdint>
#include <string>
#include <list>
#include <nlohmann\json.hpp>
#include "db_mysql.h"
#include "util.h"
#include "OrderDetail.h"


class Districts {
public:
	int16_t DistrictId;
	std::string DistrictName;
};

class Order {
public:

	//Base members mapped to db table
	int32_t OrderId;
	int32_t ClientId;
	time_t DatePlaced;
	std::string Notes;
	bool Delivered;
	bool Paid;
	time_t DateDelivered;

	std::string OrderId_uuid;

	using OrderList = std::list<Order>;

	//OrderDetails vector from database table
	std::vector<OrderDetail> products;

	//Optional fields for receiving extra data
	std::string ClientFullName;

	//db access

	// select all Orders
	OrderList selectOrders();

	//select Order by ID
	Order selectOrderById(int32_t id);
	Order selectOrderById(std::string uuid);

	//update Order
	bool updateOrder(Order order);

	//new Order
	bool newOrder(Order order);

	//delete Order
	bool deleteOrder(int32_t id);
	bool deleteOrder(std::string uuid);


	OrderList selectPendingOrders();

	std::string to_json_array(OrderList order);

	void from_json(const nlohmann::json& j, Order& s);
	void to_json(nlohmann::json& j, const Order& s);

};

#endif
