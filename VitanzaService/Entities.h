/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once

#ifndef ENTITIES_H
#define ENTITIES_H

#include "db_dynamo.h"

class Client {
public:
	// AP 1
	static void query_clients_by_status(const std::string& status, nlohmann::json& result_out);

	// AP 2
	static void get_client(const std::string& client_id, nlohmann::json& result_out);
};

class Order {
public:
	// AP 3
	static void query_orders_by_client(const std::string& client_id, nlohmann::json& result_out);

	// AP 5
	static void query_orders_by_status(const std::string& status, nlohmann::json& result_out);

	// AP 7
	static void get_order(const std::string& order_id, nlohmann::json& result_out);
};

class OrderDetail {
public:
	// AP 8
	static void get_order_details_by_order(const std::string& order_id, nlohmann::json& result_out);
};

class FilterInstallation {
public:
	// AP 4
	static void query_filter_installations_by_client(const std::string& filter_install_id, nlohmann::json& result_out);
	
	// AP 6
	static void get_filter_installation(const std::string& filter_install_id, nlohmann::json& result_out);
};

class FilterChange {
public:
	// AP 12
	static void get_changes_by_installation(const std::string& filter_install_id, nlohmann::json& result_out);

	// AP 13
	static void get_changes_by_status_dates(const std::string& status, const std::string& date_start, const std::string& date_finish, nlohmann::json& result_out);
};

class Product {
public:
	// AP 9
	static void get_product(const std::string& product_id, nlohmann::json& result_out);

	// AP 10
	static void get_current_stock(const std::string& category, nlohmann::json& result_out);
};

class Note {
public:
	// AP 11
	static void get_notes_by_status(const std::string& status, nlohmann::json& result_out);
};


#endif ENTITIES_H