#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H

#include "db_mysql.h"

class OrderDetail {
public:
	int32_t order_detail_id;
	int32_t order_id;
	std::string product;
	int32_t quantity;
	float_t price;
	float_t override_price;

	std::string order_detail_id_uuid;
	std::string order_id_uuid;

	using OrderDetailList = std::list<OrderDetail>;

#if defined(DB_MYSQL)
	//db access

	// select all OrderDetails
	static OrderDetailList select_order_details_by_order_id(const int32_t& order_id);
	static OrderDetailList select_order_details_by_order_id(const std::string& uuid);

	//select OrderDetail by ID
	static OrderDetail select_order_detail_by_id(const int32_t& id);
	static OrderDetail select_order_detail_by_id(const std::string& uuid);

	//update OrderDetail
	static bool update_order_detail(const OrderDetail& order_detail);

	//new OrderDetail
	static bool new_order_detail(const OrderDetail& order_detail);

	//delete OrderDetail
	static bool delete_order_detail(const int32_t& id);
	static bool delete_order_detail(const std::string& uuid);

	//delete OrderDetail
	static bool delete_order_detail_by_order_id(const int32_t& id);
	static bool delete_order_detail_by_order_id(const std::string& uuid);

#endif

	static std::string to_json_array(const OrderDetailList& order_detail);

	static void from_json(const nlohmann::json& j, OrderDetail& s);
	static void to_json(nlohmann::json& j, const OrderDetail& s);
};

#endif
