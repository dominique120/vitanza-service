#include "product_wrapper.h"

nlohmann::json Client_wrapper::getProduct(std::string id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    Aws::String id(id_or_uuid.c_str());
    return dyn.getItemDynamo("products", "ProductId_uuid", id);
#elif _MYSQL
    Product p;
    p.selectProductById(id_or_uuid);
    nlohmann::json j;
    p.to_json(j, p);
    return j;
#endif 
}

bool Client_wrapper::putProduct(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::deleteProduct(std::string id_or_uuid) {
    return nlohmann::json();
}

bool Client_wrapper::postProduct(std::string id_or_uuid) {
    return nlohmann::json();
}
