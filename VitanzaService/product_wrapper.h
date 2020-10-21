#ifndef PRODUCT_WRAPPER_H
#define PRODUCT_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Product.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	static nlohmann::json get_product(const std::string& id_or_uuid);
	static bool put_product(const std::string& id_or_uuid);
	static bool delete_product(const std::string& id_or_uuid);
	static bool post_product(const std::string& id_or_uuid);
};

#endif