#include "vtspch.h"
#include "Entities.h"

void Client::query_clients_by_status(const std::string& status, nlohmann::json& result_out)
{
	// GSI-2PK = ACTIVE

}

void Client::get_client(const std::string& client_id, nlohmann::json& result_out)
{
	// PK is CLI|uuid

}

void Order::query_orders_by_client(const std::string& client_id, nlohmann::json& result_out)
{
	// PK is CLI|uuid, SK starts with ORD

}

void Order::query_orders_by_status(const std::string& status, nlohmann::json& result_out)
{
	// GSI - 2 = P_PAY

}

void Order::get_order(const std::string& order_id, nlohmann::json& result_out)
{
	// GSI1-PK is ORD|uuid

}

void FilterInstallation::query_filter_installations_by_client(const std::string& filter_install_id, nlohmann::json& result_out)
{
	// PK is CLI|uuid, SK starts with FLI

}

void FilterInstallation::get_filter_installation(const std::string& filter_install_id, nlohmann::json& result_out)
{
	// GSI1-PK is FLI|uui

}

void OrderDetail::get_order_details_by_order(const std::string& order_id, nlohmann::json& result_out)
{
	// PK is ORD|uuid, SK is starts with ODD

}

void Product::get_product(const std::string& product_id, nlohmann::json& result_out)
{
	// PK is PRD|uuid

}

void Product::get_current_stock(const std::string& category, nlohmann::json& result_out)
{
	// GSI-1 PK is filters/chem, SK stats with PRO

}

void Note::get_notes_by_status(const std::string& status, nlohmann::json& result_out)
{
	// GSI-1 PK = OPEN

}

void FilterChange::get_changes_by_installation(const std::string& filter_install_id, nlohmann::json& result_out)
{
	// PK = FLI|uid, SK begins with FLC

}

void FilterChange::get_changes_by_status_dates(const std::string& status, const std::string& date_start, const std::string& date_finish, nlohmann::json& result_out)
{
	// GSI-1 PK = STATUS, GSI-1 SK in range

}
