#include "vtspch.h"
#include "product_wrapper.h"

std::string Product_wrapper::get_product(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    const Aws::String id(id_or_uuid.c_str());
    const nlohmann::json j = DynamoDB::get_item_dynamo("products", "ProductId_uuid", id);
    return j.dump();
#elif DB_MYSQL
    Product p;
    p.select_product_by_id(id_or_uuid);
    nlohmann::json j;
    p.to_json(j, p);
    return j.dump();
#endif 
}

bool Product_wrapper::delete_product(const std::string& id_or_uuid) {
#ifdef DB_DYNAMO
    const Aws::String id(id_or_uuid.c_str());
    return DynamoDB::delete_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str());
#elif DB_MYSQL
    Product p;
    return p.delete_product(id_or_uuid);
#endif 
}

bool Product_wrapper::new_product(const std::string& request_body) {
#ifdef DB_DYNAMO
    /* ---------UUID Generation -----------------*/
    const auto uuid = boost::uuids::random_generator()();
    const std::string id = boost::uuids::to_string(uuid);
    return DynamoDB::new_item_dynamo("products", "ProductId_uuid", id.c_str(), request_body);
#elif DB_MYSQL
    Product p;
    p.from_json(request_body, p);
    return p.new_product(p);
#endif 
}

bool Product_wrapper::update_product(const std::string& id_or_uuid, const std::string& request_body) {
#ifdef DB_DYNAMO
    const Aws::String id(id_or_uuid.c_str());
    return DynamoDB::update_item_dynamo("products", "ProductId_uuid", id_or_uuid.c_str(), request_body);
#elif DB_MYSQL
    Product p;
    p.from_json(request_body, p);
    return p.update_product(p);
#endif 
}

std::string Product_wrapper::get_all_products() {
#ifdef DB_DYNAMO
    return DynamoDB::scan_table_items_dynamo("products");
#elif DB_MYSQL
    // TODO: implement mysql method for getting all clients
    //Client p;
    //p.from_json(request_body, p);
    //return p.new_client(p);
    return "";
#endif
}