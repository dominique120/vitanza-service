#include "order_wrapper.h"

nlohmann::json Client_wrapper::getOrder(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    Aws::String id(id_or_uuid.c_str());
    return dyn.getItemDynamo("orders", "OrderId_uuid", id);
#elif _MYSQL
    Order o;
    o.selectOrderById(id_or_uuid);
    nlohmann::json j;
    o.to_json(j, o);
    return j;
#endif 
}

bool Client_wrapper::putOrder(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::deleteOrder(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::postOrder(std::string id_or_uuid) {
    return nlohmann::json();
}
