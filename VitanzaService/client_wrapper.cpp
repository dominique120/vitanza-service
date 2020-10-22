#include "client_wrapper.h"

std::string Client_wrapper::get_client(const std::string& id_or_uuid) {
#ifdef _DYNAMO
	DynamoDB dyn;
	const Aws::String id(id_or_uuid.c_str());
	const nlohmann::json j = dyn.get_item_dynamo("clients", "ClientId_uuid", id);
	return j.dump();
#elif _MYSQL
	Client p;
	p.select_client_by_id(id_or_uuid);
	nlohmann::json j;
	p.to_json(j, p);
	return j.dump();
#endif 
}

bool Client_wrapper::update_client(const std::string& id_or_uuid, const nlohmann::json& json_req) {
#ifdef _DYNAMO
	DynamoDB dyn;
	const Aws::String id(id_or_uuid.c_str());
	return dyn.update_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str(), json_req);
#elif _MYSQL
	Client p;
	p.from_json(json_req, p);
	return p.update_client(p);
#endif 
}

bool Client_wrapper::delete_client(const std::string& id_or_uuid) {
#ifdef _DYNAMO
	DynamoDB dyn;
	const Aws::String id(id_or_uuid.c_str());
	return dyn.delete_item_dynamo("clients", "ClientId_uuid", id_or_uuid.c_str());
#elif _MYSQL
	Client p;
	return p.delete_client(id_or_uuid);
#endif 
}

bool Client_wrapper::new_client(const nlohmann::json& json_req) {
	/* ---------UUID Generation -----------------*/
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	const std::string id = boost::uuids::to_string(uuid);

#ifdef _DYNAMO
	DynamoDB dyn;
	return dyn.new_item_dynamo("clients", "ClientId_uuid", id.c_str(), json_req);
#elif _MYSQL
	Client p;
	p.from_json(json_req, p);
	return p.new_client(p);
#endif 
}
