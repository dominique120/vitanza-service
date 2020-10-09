#ifndef CLIENT_H
#define CLIENT_H



#include <list>
#include <iostream>

#include <nlohmann/json.hpp>

#include "db_mysql.h"

class Client {

public:	
	// members
	uint32_t ClientId;
	std::string FirstName;
	std::string LastNames;
	std::string PrimaryAddress;
	std::string SecondaryAddress;
	std::string PrimaryPhone;
	std::string SecondaryPhone;
	std::string District;

	std::string ClientId_uuid;


	using ClientList = std::list<Client>;

	/*-------------- db access -----------------------*/

	// select all clients
	ClientList selectClients();

	//select client by ID
	Client selectClientById(uint32_t id);
	Client selectClientById(std::string uuid);

	//update client
	bool updateClient(Client client);

	//new client
	bool newClient(Client client);

	//delete client
	bool deleteClient(uint32_t id);
	bool deleteClient(std::string uuid);


	/* --------------- to and from json functions -----------------*/
	std::string to_json_array(ClientList cl);

	void from_json(const nlohmann::json& j, Client& s);
	void to_json(nlohmann::json& j, const Client& s);

};



#endif