/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "OrderDetail.h"

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