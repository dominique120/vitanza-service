/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "Order.h"

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