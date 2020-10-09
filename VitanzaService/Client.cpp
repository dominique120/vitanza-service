#include "Client.h"

Client::ClientList Client::selectClients() {
	ClientList returnedTable;
	std::ostringstream query;
	query << "SELECT c.`ClientId`, c.`FirstName`, c.`LastNames`, c.`PrimaryAddress`, c.`SecondaryAddress`, c.`PrimaryPhone`, c.`SecondaryPhone`, d.`DistrictName` FROM `clients` c inner join districts d on c.`District` = d.`DistrictId`;";
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return returnedTable;
	}
	do {
		Client client;
		client.ClientId = result->getNumber<uint32_t>("ClientId");
		client.FirstName = result->getString("FirstName");
		client.LastNames = result->getString("LastNames");
		client.PrimaryAddress = result->getString("PrimaryAddress");
		client.SecondaryAddress = result->getString("SecondaryAddress");
		client.PrimaryPhone = result->getString("PrimaryPhone");
		client.SecondaryPhone = result->getString("SecondaryPhone");
		client.District = result->getString("DistrictName");
		returnedTable.push_back(client);
	} while (result->next());
	return returnedTable;
}

Client Client::selectClientById(uint32_t id) {
	Client client;
	std::ostringstream query;
	query << "SELECT c.`ClientId`, c.`FirstName`, c.`LastNames`, c.`PrimaryAddress`, c.`SecondaryAddress`, c.`PrimaryPhone`, c.`SecondaryPhone`, d.`DistrictName` FROM `clients` c INNER JOIN Districts d on c.`District` = d.`DistrictId` WHERE ClientId = " << id;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return client;
	}

	client.ClientId = result->getNumber<uint32_t>("ClientId");
	client.FirstName = result->getString("FirstName").c_str();
	client.LastNames = result->getString("LastNames").c_str();
	client.PrimaryAddress = result->getString("PrimaryAddress").c_str();
	client.SecondaryAddress = result->getString("SecondaryAddress").c_str();
	client.PrimaryPhone = result->getString("PrimaryPhone").c_str();
	client.SecondaryPhone = result->getString("SecondaryPhone").c_str();
	client.District = result->getString("DistrictName").c_str();

	return client;
}

Client Client::selectClientById(std::string uuid) {
	return Client();
}

bool Client::updateClient(Client client) {
	Database db;
	std::ostringstream query;
	query << "UPDATE `clients` c SET c.`FirstName` = " << db.escapeString(client.FirstName) << ", ";
	query << "c.`LastNames` = " << db.escapeString(client.LastNames) << " ,";
	query << "c.`PrimaryAddress` = " << db.escapeString(client.PrimaryAddress) << ",";
	query << "c.`SecondaryAddress` = " << db.escapeString(client.SecondaryAddress) << ",";
	query << "c.`PrimaryPhone` = " << db.escapeString(client.PrimaryPhone) << ",";
	query << "c.`SecondaryPhone` = " << db.escapeString(client.SecondaryPhone) << ",";
	query << "c.`District` = (SELECT `districts`.`DistrictId` ";
	query << "FROM `vitanza`.`districts` WHERE `districts`.`DistrictName` = " << db.escapeString(client.District) << ") ";
	query << "WHERE `ClientId` = " << client.ClientId;

	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::newClient(Client client) {
	Database db;
	std::ostringstream query;
	query << "INSERT INTO `clients` (`FirstName`,`LastNames`,`PrimaryAddress`,`SecondaryAddress`,`PrimaryPhone`,`SecondaryPhone`, `District`) VALUES( ";
	query << db.escapeString(client.FirstName) << ", ";
	query << db.escapeString(client.LastNames) << ", ";
	query << db.escapeString(client.PrimaryAddress) << ", ";
	query << db.escapeString(client.SecondaryAddress) << ", ";
	query << db.escapeString(client.PrimaryPhone) << ", ";
	query << db.escapeString(client.SecondaryPhone) << ", ";
	query << "(SELECT `districts`.`DistrictId`FROM `vitanza`.`districts` WHERE `districts`.`DistrictName` = ";
	query << db.escapeString(client.District) << " )  ); ";
	if (db.getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::deleteClient(uint32_t id) {
	std::ostringstream query;
	query << "DELETE FROM `clients`WHERE ClientId = " << id;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

bool Client::deleteClient(std::string uuid) {
	std::ostringstream query;
	query << "DELETE FROM `clients`WHERE ClientId_uuid = " << uuid;

	if (Database::getInstance().executeQuery(query.str())) {
		return true;
	} else {
		return false;
	}
}

std::string Client::to_json_array(ClientList cl) {

	nlohmann::json j;

	for (auto const& i : cl) {
		j.push_back({
			{ "ClientId",i.ClientId},
			{ "ClientId_uuid",i.ClientId_uuid},
			{ "FirstName", i.FirstName},
			{ "LastNames",i.LastNames},
			{ "PrimaryAddress",i.PrimaryAddress},
			{ "SecondaryAddress", i.SecondaryAddress},
			{ "PrimaryPhone", i.PrimaryPhone},
			{ "SecondaryPhone",i.SecondaryPhone},
			{ "District", i.District}
				});
	}
	return j.dump();
}

void Client::to_json(nlohmann::json& j, const Client& s) {
	j [ "ClientId" ] = s.ClientId;
	j [ "FirstName" ] = s.FirstName;
	j [ "LastNames" ] = s.LastNames;
	j [ "PrimaryAddress" ] = s.PrimaryAddress;
	j [ "SecondaryAddress" ] = s.SecondaryAddress;
	j [ "PrimaryPhone" ] = s.PrimaryPhone;
	j [ "SecondaryAddress" ] = s.SecondaryPhone;
	j [ "District" ] = s.District;
	j [ "ClientId_uuid" ] = s.ClientId_uuid;
}

void Client::from_json(const nlohmann::json& j, Client& s) {

	try {
		s.ClientId = std::stoi((j.at("ClientId").get<std::string>()));
		s.ClientId_uuid = j.at("ClientId_uuid").get<std::string>();
	} catch (nlohmann::json::exception) {
		// Expected Behavior
	}

	try {
		s.FirstName = j.at("FirstName").get<std::string>();
		s.LastNames = j.at("LastNames").get<std::string>();
		s.PrimaryAddress = j.at("PrimaryAddress").get<std::string>();
		s.SecondaryAddress = j.at("SecondaryAddress").get<std::string>();
		s.PrimaryPhone = j.at("PrimaryPhone").get<std::string>();
		s.SecondaryPhone = j.at("SecondaryPhone").get<std::string>();
		s.District = j.at("District").get<std::string>();
	} catch (nlohmann::json::exception) {
		std::cout << "Client sent with missing mandatory data" << std::endl;
	}
}