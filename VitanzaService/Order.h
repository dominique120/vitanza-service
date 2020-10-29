#ifndef ORDER_H
#define ORDER_H


#include "db_mysql.h"
#include "util.h"
#include "OrderDetail.h"


class Districts {
public:
	int16_t district_id;
	std::string district_name;
};

class Order {
public:

	//Base members mapped to db table
	int32_t order_id;
	int32_t client_id;
	time_t date_placed;
	std::string notes;
	bool delivered;
	bool paid;
	time_t date_delivered;

	std::string order_id_uuid;



	using OrderList = std::list<Order>;
#if defined(DB_MYSQL)
	//OrderDetails vector from database table
	std::vector<OrderDetail> products;

	//Optional fields for receiving extra data
	std::string ClientFullName;

	//db access

	// select all Orders
	static OrderList select_orders();

	//select Order by ID
	static Order select_order_by_id(const int32_t& id);
	static Order select_order_by_id(const std::string& uuid);

	//update Order
	static bool update_order(const Order& order);

	//new Order
	static bool new_order(const Order& order);

	//delete Order
	static bool delete_order(const int32_t& id);
	static bool delete_order(const std::string& uuid);


	static OrderList select_pending_orders();
#endif
	static std::string to_json_array(const OrderList& order);

	static void from_json(const nlohmann::json& j, Order& s);
	static void to_json(nlohmann::json& j, const Order& s);

};

#endif
