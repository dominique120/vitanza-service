/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CHEMICAL_ORDERDETAIL_WRAPPER_H
#define CHEMICAL_ORDERDETAIL_WRAPPER_H

#include "db_dynamo.h"

class Chemical_Order_Detail_Wrapper {
public:
	static std::string get_order_detail(const std::string& id_or_uuid);
	static bool delete_order_detail(const std::string& id_or_uuid);
	static bool new_order_detail(const std::string& request_body);
	static bool update_order_detail(const std::string& id_or_uuid, const std::string& request_body);
	static std::string get_orderdetails_by_order(const std::string& id_or_uuid);
};

#endif