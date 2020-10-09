
#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H
#include <iostream>
#include <cstdint>
#include <sstream>
#include <list>
#include <nlohmann\json.hpp>
#include "db_mysql.h"

class OrderDetail {
public:
	int32_t OrderDetailId;
	int32_t OrderId;
	std::string Product;
	int32_t Quantity;
	float_t Price;
	float_t OverridePrice;

	std::string OrderDetailId_uuid;
	std::string OrderId_uuid;

	using OrderDetailList = std::list<OrderDetail>;

	//db access

	// select all OrderDetails
	OrderDetailList selectOrderDetailsByOrderId(int32_t orderId);
	OrderDetailList selectOrderDetailsByOrderId(std::string uuid);

	//select OrderDetail by ID
	OrderDetail selectOrderDetailById(int32_t id);
	OrderDetail selectOrderDetailById(std::string uuid);

	//update OrderDetail
	bool updateOrderDetail(OrderDetail orderDetail);

	//new OrderDetail
	bool newOrderDetail(OrderDetail orderDetail);

	//delete OrderDetail
	bool deleteOrderDetail(int32_t id);
	bool deleteOrderDetail(std::string uuid);

	//delete OrderDetail
	bool deleteOrderDetailByOrderId(int32_t id);
	bool deleteOrderDetailByOrderId(std::string uuid);

	std::string to_json_array(OrderDetailList orderDetail);

	void from_json(const nlohmann::json& j, OrderDetail& s);
	void to_json(nlohmann::json& j, const OrderDetail& s);
};

#endif



