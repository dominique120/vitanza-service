#include "vtspch.h"
#include "Entities.h"

void Client::query_clients_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI2PK = ACTIVE
	nlohmann::json j;
	j["status"] = status;
	DynamoDB::query_with_expression("Vitanza", "GSI2PK", "GSI2PK = :status", j, result_out);
}

void Client::get_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid
	const Aws::String PK = client_id.c_str();
	DynamoDB::get_item_composite("Vitanza", "PK", PK, "SK", PK, result_out);
}

void Order::query_orders_by_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK starts with ORD
	nlohmann::json j;
	j["PK"] = client_id;
	j["order"] = "ORD";
	DynamoDB::query_with_expression("Vitanza", "", "PK = :PK and begins_with(SK, :order)", j, result_out);
}

void Order::query_orders_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI2PK = P_PAY
	nlohmann::json j;
	j["status"] = status;
	DynamoDB::query_with_expression("Vitanza", "GSI2PK", "GSI2PK = :status", j, result_out);
}

void Order::get_order(const std::string& client_id, const std::string& order_id, nlohmann::json& result_out) {
	// GSI1PK is ORD|uuid
	const Aws::String PK = client_id.c_str();
	const Aws::String SK = order_id.c_str();
	DynamoDB::get_item_composite("Vitanza", "PK", PK, "SK", SK, result_out);
}

void FilterInstallation::query_filter_installations_by_client(const std::string& client_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK starts with FLI
	nlohmann::json j;
	j["PK"] = client_id;
	j["fli"] = "FLI";
	DynamoDB::query_with_expression("Vitanza", "", "PK = :PK and begins_with(SK, :fli)", j, result_out);
}

void FilterInstallation::get_filter_installation(const std::string& client_id, const std::string& filter_install_id, nlohmann::json& result_out) {
	// PK is CLI|uuid, SK is FLI|uuid
	const Aws::String PK = client_id.c_str();
	const Aws::String SK = filter_install_id.c_str();
	std::cout << "PK = " << PK << " SK = " << SK << "\n";
	DynamoDB::get_item_composite("Vitanza", "PK", PK, "SK", SK, result_out);
}

void OrderDetail::get_order_details_by_order(const std::string& order_id, nlohmann::json& result_out) {
	// PK is ORD|uuid, SK is starts with ODD
	nlohmann::json j;
	j["PK"] = order_id;
	j["odd"] = "ODD";
	DynamoDB::query_with_expression("Vitanza", "", "PK = :PK and begins_with(SK, :odd)", j, result_out);
}

void Product::get_product(const std::string& category, const std::string& product_id, nlohmann::json& result_out) {
	// PK is PRD|uuid
	const Aws::String PK = product_id.c_str();
	const Aws::String SK = category.c_str();
	DynamoDB::get_item_composite("Vitanza", "PK", PK, "SK", SK, result_out);
}

void Product::get_current_stock(const std::string& category, nlohmann::json& result_out) {
	// GSI1 PK is filters/chem, SK stats with PRO
	nlohmann::json j;
	j["gsi"] = category;
	j["pro"] = "PRO";
	DynamoDB::query_with_expression("Vitanza", "GSI1PK", "GSI1PK = :gsi and begins_with(GSI1SK, :pro)", j, result_out);
}

void Note::get_notes_by_status(const std::string& status, nlohmann::json& result_out) {
	// GSI1 PK = OPEN
	nlohmann::json j;
	j["status"] = status;
	DynamoDB::query_with_expression("Vitanza", "GSI1PK", "GSI1PK = :status", j, result_out);
}

void FilterChange::get_changes_by_installation(const std::string& filter_install_id, nlohmann::json& result_out) {
	// PK = FLI|uid, SK begins with FLC
	nlohmann::json j;
	j["pk"] = filter_install_id;
	j["flc"] = "FLC";
	DynamoDB::query_with_expression("Vitanza", "", "PK = :pk and begins_with(SK, :flc)", j, result_out);
}

void FilterChange::get_changes_by_status_dates(const std::string& status, const std::string& date_start, const std::string& date_finish, nlohmann::json& result_out) {
	// GSI1 PK = STATUS, GSI1 SK in range
	nlohmann::json j;
	j["status"] = status;
	j["start"] = date_start;
	j["end"] = date_finish;
	DynamoDB::query_with_expression("Vitanza", "GSI1PK", "GSI1PK = :status AND GSI1SK BETWEEN :start AND :end", j, result_out);
	// #DocTimestamp BETWEEN :start AND :end
}
