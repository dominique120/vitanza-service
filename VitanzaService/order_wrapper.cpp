#include "vtspch.h"
#include "order_wrapper.h"

std::string Order_wrapper::get_order(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    const nlohmann::json j = dyn.get_item_dynamo("orders", "OrderId_uuid", id);
    return j.dump();
#elif DB_MYSQL
    Order o;
    o.select_order_by_id(id_or_uuid);
    nlohmann::json j;
    o.to_json(j, o);
    return j.dump();
#endif 
}

bool Order_wrapper::update_order(const std::string& id_or_uuid, const std::string& request_body) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.update_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str(), request_body);
#elif DB_MYSQL
    Order o;
    o.from_json(request_body, o);
    return o.update_order(o);
#endif 
}

bool Order_wrapper::delete_order(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.delete_item_dynamo("orders", "OrderId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
    Order o;
    return o.delete_order(id_or_uuid);
#endif 
}

bool Order_wrapper::new_order(const std::string& request_body) {
#ifdef DB_DYNAMO
    /* ---------UUID Generation -----------------*/
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const std::string id = boost::uuids::to_string(uuid);
    DynamoDB dyn;
    return dyn.new_item_dynamo("orders", "OrderId_uuid", id.c_str(), request_body);
#elif DB_MYSQL
    Order o;
    o.from_json(request_body, o);
    return o.new_order(o);
#endif 
}

std::string Order_wrapper::get_all_orders() {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    return dyn.scan_table_items_dynamo("orders");
#elif DB_MYSQL
    // TODO: implement mysql method for getting all clients
    //Client p;
    //p.from_json(request_body, p);
    //return p.new_client(p);
    return "";
#endif
}

std::string Order_wrapper::get_outstanding_orders() {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    std::map<std::string, std::string> conditions;
    auto pending = std::pair<std::string, std::string>("Pending", "0");
    conditions.insert(pending);
    auto paid = std::pair<std::string, std::string>("Paid", "0");
    conditions.insert(paid);

    return dyn.scan_table_items_filer_dynamo("orders", conditions);
#elif DB_MYSQL
    // TODO: implement mysql method for getting all clients
    //Client p;
    //p.from_json(request_body, p);
    //return p.new_client(p);
    return "";
#endif
}
