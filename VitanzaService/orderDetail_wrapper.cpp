#include "orderDetail_wrapper.h"

nlohmann::json Client_wrapper::get_order_detail(const std::string& id_or_uuid) {
#ifdef _DYNAMO
        DynamoDB dyn;
        const Aws::String id(id_or_uuid.c_str());
    return dyn.get_item_dynamo("orderdetails", "OrderDetailId_uuid", id);
#elif _MYSQL
    OrderDetail od;
    od.selectOrderDetailById(id_or_uuid);
    nlohmann::json j;
    od.to_json(j, od);
    return j;
#endif 
}

bool Client_wrapper::put_order_detail(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::delete_order_detail(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::post_order_detail(const std::string& id_or_uuid) {
    return nlohmann::json();
}
