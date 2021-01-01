/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H

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

	static std::string to_json_array(const OrderDetailList& order_detail);

	static void from_json(const nlohmann::json& j, OrderDetail& s);
	static void to_json(nlohmann::json& j, const OrderDetail& s);
};

#endif
