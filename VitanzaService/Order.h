/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef ORDER_H
#define ORDER_H

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

	static std::string to_json_array(const OrderList& order);

	static void from_json(const nlohmann::json& j, Order& s);
	static void to_json(nlohmann::json& j, const Order& s);
};

#endif
