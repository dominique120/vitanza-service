#include "Client.h"
#if defined(DB_MYSQL)
Client::ClientList Client::select_clients() {
	ClientList returned_table;
	std::ostringstream query;
	query << "SELECT c.`ClientId`, c.`FirstName`, c.`LastNames`, c.`PrimaryAddress`, c.`SecondaryAddress`, c.`PrimaryPhone`, c.`SecondaryPhone`, d.`DistrictName` FROM `clients` c inner join districts d on c.`District` = d.`DistrictId`;";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returned_table;
	}
	do {
		Client client;
		client.client_id = result->getNumber<uint32_t>("ClientId");
		client.first_name = result->getString("FirstName");
		client.last_names = result->getString("LastNames");
		client.primary_address = result->getString("PrimaryAddress");
		client.secondary_address = result->getString("SecondaryAddress");
		client.primary_phone = result->getString("PrimaryPhone");
		client.secondary_phone = result->getString("SecondaryPhone");
		client.district = result->getString("DistrictName");
		returned_table.push_back(client);
	} while (result->next());
	return returned_table;
}

Client Client::select_client_by_id(const uint32_t& id) {
	Client client;
	std::ostringstream query;
	query << "SELECT c.`ClientId`, c.`FirstName`, c.`LastNames`, c.`PrimaryAddress`, c.`SecondaryAddress`, c.`PrimaryPhone`, c.`SecondaryPhone`, d.`DistrictName` FROM `clients` c INNER JOIN Districts d on c.`District` = d.`DistrictId` WHERE ClientId = " << id;

	const DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		client.client_id = -1;
		return client;
	}

	client.client_id = result->getNumber<uint32_t>("ClientId");
	client.first_name = result->getString("FirstName");
	client.last_names = result->getString("LastNames");
	client.primary_address = result->getString("PrimaryAddress");
	client.secondary_address = result->getString("SecondaryAddress");
	client.primary_phone = result->getString("PrimaryPhone");
	client.secondary_phone = result->getString("SecondaryPhone");
	client.district = result->getString("DistrictName");

	return client;
}

Client Client::select_client_by_id(const std::string& uuid) {
	return Client();
}

bool Client::update_client(const Client& client) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `clients` c SET c.`FirstName` = " << db.escapeString(client.first_name) << ", ";
	query << "c.`LastNames` = " << db.escapeString(client.last_names) << " ,";
	query << "c.`PrimaryAddress` = " << db.escapeString(client.primary_address) << ",";
	query << "c.`SecondaryAddress` = " << db.escapeString(client.secondary_address) << ",";
	query << "c.`PrimaryPhone` = " << db.escapeString(client.primary_phone) << ",";
	query << "c.`SecondaryPhone` = " << db.escapeString(client.secondary_phone) << ",";
	query << "c.`District` = (SELECT `districts`.`DistrictId` ";
	query << "FROM `vitanza`.`districts` WHERE `districts`.`DistrictName` = " << db.escapeString(client.district) << ") ";
	query << "WHERE `ClientId` = " << client.client_id;

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::new_client(const Client& client) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `clients` (`FirstName`,`LastNames`,`PrimaryAddress`,`SecondaryAddress`,`PrimaryPhone`,`SecondaryPhone`, `District`) VALUES( ";
	query << db.escapeString(client.first_name) << ", ";
	query << db.escapeString(client.last_names) << ", ";
	query << db.escapeString(client.primary_address) << ", ";
	query << db.escapeString(client.secondary_address) << ", ";
	query << db.escapeString(client.primary_phone) << ", ";
	query << db.escapeString(client.secondary_phone) << ", ";
	query << "(SELECT `districts`.`DistrictId`FROM `vitanza`.`districts` WHERE `districts`.`DistrictName` = ";
	query << db.escapeString(client.district) << " )  ); ";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::delete_client(const uint32_t& id) {
	std::ostringstream query;
	query << "DELETE FROM `clients`WHERE ClientId = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::delete_client(const std::string& uuid)
{
	std::ostringstream query;
	query << "DELETE FROM `clients`WHERE ClientId_uuid = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}
#endif

std::string Client::to_json_array(const ClientList& cl) {

	nlohmann::json j;

	for (auto const& i : cl) {
		j.push_back({
			{ "ClientId",i.client_id},
			{ "ClientId_uuid",i.client_id_uuid},
			{ "FirstName", i.first_name},
			{ "LastNames",i.last_names},
			{ "PrimaryAddress",i.primary_address},
			{ "SecondaryAddress", i.secondary_address},
			{ "PrimaryPhone", i.primary_phone},
			{ "SecondaryPhone",i.secondary_phone},
			{ "District", i.district}
				});
	}
	return j.dump();
}

void Client::to_json(nlohmann::json& j, const Client& s) {
	j [ "ClientId" ] = s.client_id;
	j [ "FirstName" ] = s.first_name;
	j [ "LastNames" ] = s.last_names;
	j [ "PrimaryAddress" ] = s.primary_address;
	j [ "SecondaryAddress" ] = s.secondary_address;
	j [ "PrimaryPhone" ] = s.primary_phone;
	j [ "SecondaryAddress" ] = s.secondary_phone;
	j [ "District" ] = s.district;
	j [ "ClientId_uuid" ] = s.client_id_uuid;
}

void Client::from_json(const nlohmann::json& j, Client& s) const
{

	try {
		s.client_id = std::stoi((j.at("ClientId").get<std::string>()));
		s.client_id_uuid = j.at("ClientId_uuid").get<std::string>();
	} catch (nlohmann::json::exception&) {
		// Expected Behavior
	}

	try {
		s.first_name = j.at("FirstName").get<std::string>();
		s.last_names = j.at("LastNames").get<std::string>();
		s.primary_address = j.at("PrimaryAddress").get<std::string>();
		s.secondary_address = j.at("SecondaryAddress").get<std::string>();
		s.primary_phone = j.at("PrimaryPhone").get<std::string>();
		s.secondary_phone = j.at("SecondaryPhone").get<std::string>();
		s.district = j.at("District").get<std::string>();
	} catch (nlohmann::json::exception&) {
		std::cout << "Client sent with missing mandatory data" << std::endl;
	}
}
