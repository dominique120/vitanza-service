#include "product_wrapper.h"

nlohmann::json Client_wrapper::get_product(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    Aws::String id(id_or_uuid.c_str());
    return dyn.get_item_dynamo("products", "ProductId_uuid", id);
#elif _MYSQL
    Product p;
    p.selectProductById(id_or_uuid);
    nlohmann::json j;
    p.to_json(j, p);
    return j;
#endif 
}

bool Client_wrapper::put_product(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::delete_product(const std::string& id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::post_product(const std::string& id_or_uuid) {
    return nlohmann::json();
}
