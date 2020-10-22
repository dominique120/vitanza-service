#include "order_wrapper.h"

std::string Order_wrapper::get_order(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    const nlohmann::json j = dyn.get_item_dynamo("orders", "OrderId_uuid", id);
    return j.dump();
#elif _MYSQL
    Order o;
    o.select_order_by_id(id_or_uuid);
    nlohmann::json j;
    o.to_json(j, o);
    return j.dump();
#endif 
}

bool Order_wrapper::update_order(const std::string& id_or_uuid, const nlohmann::json& json_req) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.update_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str(), json_req);
#elif _MYSQL
    Order o;
    o.from_json(json_req, o);
    return o.update_order(o);
#endif 
}

bool Order_wrapper::delete_order(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.delete_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str());
#elif _MYSQL
    Order o;
    return o.delete_order(id_or_uuid);
#endif 
}

bool Order_wrapper::new_order(const nlohmann::json& json_req) {
#ifdef _DYNAMO
    /* ---------UUID Generation -----------------*/
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const std::string id = boost::uuids::to_string(uuid);
    DynamoDB dyn;
    return dyn.new_item_dynamo("orders", "OrderId_uuid", id.c_str(), json_req);
#elif _MYSQL
    Order o;
    o.from_json(json_req, o);
    return o.new_order(o);
#endif 
}
