#include "client_wrapper.h"

std::string Client_wrapper::get_client(const std::string& id_or_uuid) {

#ifdef _DYNAMO
	DynamoDB dyn;
	Aws::String id(id_or_uuid.c_str());
	nlohmann::json j = dyn.get_item_dynamo("clients", "ClientId_uuid", id);
	return j.dump();
#elif _MYSQL
	Client p;
	p.selectClientById(id_or_uuid);
	nlohmann::json j;
	p.to_json(j, p);
	return j.dump();
#endif 

}

bool Client_wrapper::update_client(const std::string& id_or_uuid) {
	return nlohmann::json();
}

bool Client_wrapper::delete_client(const std::string& id_or_uuid) {
	return nlohmann::json();
}

bool Client_wrapper::new_client(const std::string& id_or_uuid) {
	//need to generate UUID for primary key and add it to the json request before sending it to dynamo
	return nlohmann::json();
}
