#include "orderDetail_wrapper.h"

std::string OrderDetail_wrapper::get_order_detail(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    const nlohmann::json j = dyn.get_item_dynamo("orderdetails", "OrderDetailId_uuid", id);
    return j.dump();
#elif DB_MYSQL
    OrderDetail od;
    od.select_order_detail_by_id(id_or_uuid);
    nlohmann::json j;
    od.to_json(j, od);
    return j.dump();
#endif 
}

bool OrderDetail_wrapper::delete_order_detail(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.delete_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
    OrderDetail o;
    return o.delete_order_detail(id_or_uuid);
#endif 
}

bool OrderDetail_wrapper::new_order_detail(const std::string& request_body) {
#ifdef DB_DYNAMO
    /* ---------UUID Generation -----------------*/
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const std::string id = boost::uuids::to_string(uuid);
    DynamoDB dyn;
    return dyn.new_item_dynamo("orderdetails", "OrderDetailId_uuid", id.c_str(), request_body);
#elif DB_MYSQL
    OrderDetail od;
    od.from_json(request_body, od);
    return od.new_order_detail(od);
#endif 
}

bool OrderDetail_wrapper::update_order_detail(const std::string& id_or_uuid, const std::string& request_body) {
#ifdef DB_DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.update_item_dynamo("orderdetails", "OrderDetailId_uuid", id_or_uuid.c_str(), request_body);
#elif DB_MYSQL
    OrderDetail od;
    od.from_json(request_body, od);
    return od.update_order_detail(od);
#endif 
}


