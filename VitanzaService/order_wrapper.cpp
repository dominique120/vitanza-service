#include "order_wrapper.h"

nlohmann::json Client_wrapper::get_order(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.get_item_dynamo("orders", "OrderId_uuid", id);
#elif _MYSQL
    Order o;
    o.selectOrderById(id_or_uuid);
    nlohmann::json j;
    o.to_json(j, o);
    return j;
#endif 
}

bool Client_wrapper::put_order(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::delete_order(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::post_order(const std::string& id_or_uuid) {
    return nlohmann::json();
}
