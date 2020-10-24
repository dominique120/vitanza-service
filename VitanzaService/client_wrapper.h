#ifndef CLIENT_WRAPPER_H
#define CLIENT_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Client.h"
#include "db_dynamo.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Client_wrapper{
public:
	static std::string get_client(const std::string& id_or_uuid);
	static bool update_client(const std::string& id_or_uuid, const std::string& request_body);
	static bool delete_client(const std::string& id_or_uuid);
	static bool new_client(const std::string& request_body);
};

#endif