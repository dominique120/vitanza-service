/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CHEMICAL_PRODUCT_WRAPPER_H
#define CHEMICAL_PRODUCT_WRAPPER_H

#include "db_dynamo.h"

class Chemical_Product_Wrapper {
public:
	static std::string get_product(const std::string& id_or_uuid);
	static bool delete_product(const std::string& id_or_uuid);
	static bool new_product(const std::string& request_body);
	static bool update_product(const std::string& id_or_uuid, const std::string& request_body);
	static std::string get_all_products();
};

#endif