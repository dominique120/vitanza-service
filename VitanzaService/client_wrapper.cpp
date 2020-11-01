#include "vtspch.h"
#include "client_wrapper.h"

std::string Client_wrapper::get_client(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	const nlohmann::json j = DynamoDB::get_item_dynamo("clients", "ClientId_uuid", id);
	return j.dump();
#elif DB_MYSQL
	Client p;
	p.select_client_by_id(id_or_uuid);
	nlohmann::json j;
	p.to_json(j, p);
	return j.dump();
#endif 
}

bool Client_wrapper::update_client(const std::string& id_or_uuid, const std::string& request_body) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	return DynamoDB::update_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str(), request_body);
#elif DB_MYSQL
	Client p;
	p.from_json(request_body, p);
	return p.update_client(p);
#endif 
}

bool Client_wrapper::delete_client(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
	const Aws::String id(id_or_uuid.c_str());
	return DynamoDB::delete_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
	Client p;
	return p.delete_client(id_or_uuid);
#endif 
}

bool Client_wrapper::new_client(const std::string& request_body) {
#ifdef DB_DYNAMO
	/* ---------UUID Generation -----------------*/
	const auto uuid = boost::uuids::random_generator()();
	const std::string id = boost::uuids::to_string(uuid);
	return DynamoDB::new_item_dynamo("clients", "ClientId_uuid", id.c_str(), request_body);
#elif DB_MYSQL
	Client p;
	p.from_json(request_body, p);
	return p.new_client(p);
#endif 
}

std::string Client_wrapper::get_all_clients() {
#ifdef DB_DYNAMO
	return DynamoDB::scan_table_items_dynamo("clients");
#elif DB_MYSQL
	// TODO: implement mysql method for getting all clients
	//Client p;
	//p.from_json(request_body, p);
	//return p.new_client(p);
	return "";
#endif 
}
