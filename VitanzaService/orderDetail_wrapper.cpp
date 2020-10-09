#include "orderDetail_wrapper.h"

nlohmann::json Client_wrapper::getOrderDetail(const std::string& id_or_uuid) {
#ifdef _DYNAMO
        DynamoDB dyn;
    Aws::String id(id_or_uuid.c_str());
    return dyn.getItemDynamo("orderdetails", "OrderDetailId_uuid", id);
#elif _MYSQL
    OrderDetail od;
    od.selectOrderDetailById(id_or_uuid);
    nlohmann::json j;
    od.to_json(j, od);
    return j;
#endif 
}

bool Client_wrapper::putOrderDetail(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::deleteOrderDetail(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::postOrderDetail(std::string id_or_uuid) {
    return nlohmann::json();
}
