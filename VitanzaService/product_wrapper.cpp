#include "product_wrapper.h"


std::string Product_wrapper::get_product(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    const nlohmann::json j = dyn.get_item_dynamo("products", "ProductId_uuid", id);
    return j.dump();
#elif _MYSQL
    Product p;
    p.select_product_by_id(id_or_uuid);
    nlohmann::json j;
    p.to_json(j, p);
    return j.dump();
#endif 
}

bool Product_wrapper::delete_product(const std::string& id_or_uuid) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.delete_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str());
#elif _MYSQL
    Product p;
    return p.delete_product(id_or_uuid);
#endif 
}

bool Product_wrapper::new_product(const nlohmann::json& json_req) {
#ifdef _DYNAMO
    /* ---------UUID Generation -----------------*/
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const std::string id = boost::uuids::to_string(uuid);
    DynamoDB dyn;
    return dyn.new_item_dynamo("products", "ProductId_uuid", id.c_str(), json_req);
#elif _MYSQL
    Product p;
    p.from_json(json_req, p);
    return p.new_product(p);
#endif 
}

bool Product_wrapper::update_product(const std::string& id_or_uuid, const nlohmann::json& json_req) {
#ifdef _DYNAMO
    DynamoDB dyn;
    const Aws::String id(id_or_uuid.c_str());
    return dyn.update_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str(), json_req);
#elif _MYSQL
    Product p;
    p.from_json(json_req, p);
    return p.update_product(p);
#endif 
}
