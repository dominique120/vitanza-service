/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "Entities.h"

void Client::query_clients_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI2PK = ACTIVE
	nlohmann::json j;
	j["status"] = status;
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "GSI2PK", "GSI2PK = :status", j, result_out);
}

void Client::get_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = client_id;

	alddb::DynamoDB::get_item_composite(Util::ddb_vts_cli(), "Vitanza", pk, result_out);
}

bool Client::new_client(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool Client::update_client(const std::string& client_id, const nlohmann::json& request) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = client_id;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), request, "Vitanza", pk);
}

void Order::query_orders_by_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK starts with ORD
	nlohmann::json j;
	j["PK"] = client_id;
	j["order"] = "ORD";
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "", "PK = :PK and begins_with(SK, :order)", j, result_out);
}

void Order::query_orders_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI2PK = P_PAY
	nlohmann::json j;
	j["status"] = status;
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "GSI2PK", "GSI2PK = :status", j, result_out);
}

void Order::get_order(const std::string& client_id, const std::string& order_id, nlohmann::json& result_out) {
	// GSI1PK is ORD|uuid
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = order_id;
	alddb::DynamoDB::get_item_composite(Util::ddb_vts_cli(), "Vitanza", pk, result_out);
}

bool Order::new_order(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool Order::update_order(const std::string& client_id, const std::string& order_id, const nlohmann::json& request) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = order_id;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), request, "Vitanza", pk);
}

void FilterInstallation::query_filter_installations_by_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK starts with FLI
	nlohmann::json j;
	j["PK"] = client_id;
	j["fli"] = "FLI";
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "", "PK = :PK and begins_with(SK, :fli)", j, result_out);
}

void FilterInstallation::get_filter_installation(const std::string& client_id, const std::string& filter_install_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK is FLI|uuid
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = filter_install_id;
	alddb::DynamoDB::get_item_composite(Util::ddb_vts_cli(), "Vitanza", pk, result_out);
}

bool FilterInstallation::new_filter_installation(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool FilterInstallation::update_filter_installation(const std::string& client_id, const std::string& filter_install_id, const nlohmann::json& request) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = client_id;
	pk.sk_name = "SK";
	pk.sk_value = filter_install_id;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), request, "Vitanza", pk);
}

void OrderDetail::get_order_details_by_order(const std::string& order_id, nlohmann::json& result_out) {
	// PK is ORD|uuid, SK is starts with ODD
	nlohmann::json j;
	j["PK"] = order_id;
	j["odd"] = "ODD";
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "", "PK = :PK and begins_with(SK, :odd)", j, result_out);
}

bool OrderDetail::new_order_detail(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool OrderDetail::remove_order_detail(const std::string& orderid, const std::string& orderdetailid) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = orderid;
	pk.sk_name = "SK";
	pk.sk_value = orderdetailid;
	return alddb::DynamoDB::delete_item_composite(Util::ddb_vts_cli(), "Vitanza", pk);
}

void Product::get_product(const std::string& category, const std::string& product_id, nlohmann::json& result_out) {
	// PK is PRD|uuid
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = product_id;
	pk.sk_name = "SK";
	pk.sk_value = category;
	alddb::DynamoDB::get_item_composite(Util::ddb_vts_cli(), "Vitanza", pk, result_out);
}

void Product::get_current_stock(const std::string& category, nlohmann::json& result_out) {
	// GSI1 PK is filters/chem, SK stats with PRO
	nlohmann::json j;
	j["gsi"] = category;
	j["pro"] = "PRO";
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "GSI1PK", "GSI1PK = :gsi and begins_with(GSI1SK, :pro)", j, result_out);
}

bool Product::new_product(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool Product::update_product(const std::string& productid, const std::string& type, const nlohmann::json& request) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = productid;
	pk.sk_name = "SK";
	pk.sk_value = type;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), request, "Vitanza", pk);
}

void Note::get_notes_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI1 PK = OPEN
	nlohmann::json j;
	j["status"] = status;
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "GSI1PK", "GSI1PK = :status", j, result_out);
}

bool Note::new_note(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool Note::update_note(const std::string& clientid, const std::string& noteid, const nlohmann::json& data) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = clientid;
	pk.sk_name = "SK";
	pk.sk_value = noteid;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), data, "Vitanza", pk);
}

void FilterChange::get_changes_by_installation(const std::string& filter_install_id, nlohmann::json& result_out) {
	// PK = FLI|uid, SK begins with FLC
	nlohmann::json j;
	j["pk"] = filter_install_id;
	j["flc"] = "FLC";
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "", "PK = :pk and begins_with(SK, :flc)", j, result_out);
}

void FilterChange::get_changes_by_status_dates(const std::string& status, const std::string& date_start, const std::string& date_finish, nlohmann::json& result_out) {
	// GSI1 PK = STATUS, GSI1 SK in range
	nlohmann::json j;
	j["status"] = status;
	j["start"] = date_start;
	j["end"] = date_finish;
	alddb::DynamoDB::query_with_expression(Util::ddb_vts_cli(), "Vitanza", "GSI1PK", "GSI1PK = :status AND GSI1SK BETWEEN :start AND :end", j, result_out);
	// #DocTimestamp BETWEEN :start AND :end
}

bool FilterChange::new_filter_change(const nlohmann::json& request) {
	return alddb::DynamoDB::put_item(Util::ddb_vts_cli(), request, "Vitanza");
}

bool FilterChange::update_filter_change(const std::string& filterid, const std::string& filterchange, const std::string& status) {
	alddb::DynamoDB::CompositePK pk;
	pk.pk_name = "PK";
	pk.pk_value = filterid;
	pk.sk_name = "SK";
	pk.sk_value = filterchange;
	return alddb::DynamoDB::update_item_composite(Util::ddb_vts_cli(), status, "Vitanza", pk);
}


