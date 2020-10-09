#ifndef CLIENT_WRAPPER_H
#define CLIENT_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Client.h"
#include "db_dynamo.h"

class Client_wrapper{
public:
	std::string getClient(const std::string& id_or_uuid);
	bool updateClient(std::string id_or_uuid);
	bool deleteClient(std::string id_or_uuid);
	bool newClient(std::string id_or_uuid);
};

#endif