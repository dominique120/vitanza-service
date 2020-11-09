/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "orderDetail_wrapper.h"

std::string OrderDetail_wrapper::get_order_detail(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	const nlohmann::json j = DynamoDB::get_item_dynamo("orderdetails", "OrderDetailId_uuid", id);
	return j.dump();
#elif DB_MYSQL
	OrderDetail od;
	od.select_order_detail_by_id(id_or_uuid);
	nlohmann::json j;
	od.to_json(j, od);
	return j.dump();
#endif
}

bool OrderDetail_wrapper::delete_order_detail(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	return DynamoDB::delete_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
	OrderDetail o;
	return o.delete_order_detail(id_or_uuid);
#endif
}

bool OrderDetail_wrapper::new_order_detail(const std::string& request_body) {
#ifdef DB_DYNAMO
	/* ---------UUID Generation -----------------*/
	const auto uuid = boost::uuids::random_generator()();
	const std::string id = boost::uuids::to_string(uuid);
	return DynamoDB::new_item_dynamo("orderdetails", "OrderDetailId_uuid", id.c_str(), request_body);
#elif DB_MYSQL
	OrderDetail od;
	od.from_json(request_body, od);
	return od.new_order_detail(od);
#endif
}

bool OrderDetail_wrapper::update_order_detail(const std::string& id_or_uuid, const std::string& request_body) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	return DynamoDB::update_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str(), request_body);
#elif DB_MYSQL
	OrderDetail od;
	od.from_json(request_body, od);
	return od.update_order_detail(od);
#endif
}

std::string OrderDetail_wrapper::get_orderdetails_by_order(const std::string& id_or_uuid) {
	
#ifdef DB_DYNAMO
	return DynamoDB::query_index("orderdetails", "OrderId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
	//TODO: Implement method for mysql
#endif
}
