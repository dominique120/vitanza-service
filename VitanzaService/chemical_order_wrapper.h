/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CHEMICAL_ORDER_WRAPPER_H
#define CHEMICAL_ORDER_WRAPPER_H

#include "db_dynamo.h"

class Chemical_Order_Wrapper {
public:
	static std::string get_order(const std::string& id_or_uuid);
	static bool delete_order(const std::string& id_or_uuid);
	static bool new_order(const std::string& request_body);
	static bool update_order(const std::string& id_or_uuid, const std::string& request_body);
	static std::string get_all_orders();
	static std::string get_outstanding_orders();
	static std::string get_order_by_client(const std::string& client_uuid);
};

#endif