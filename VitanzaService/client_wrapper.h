#ifndef CLIENT_WRAPPER_H
#define CLIENT_WRAPPER_H


#include "Client.h"
#include "db_dynamo.h"



class Client_wrapper{
public:
	static std::string get_client(const std::string& id_or_uuid);
	static bool update_client(const std::string& id_or_uuid, const std::string& request_body);
	static bool delete_client(const std::string& id_or_uuid);
	static bool new_client(const std::string& request_body);
	static std::string get_all_clients();
};

#endif