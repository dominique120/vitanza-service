#ifndef PRODUCT_WRAPPER_H
#define PRODUCT_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Product.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	nlohmann::json getProduct(std::string id_or_uuid);
	bool putProduct(std::string id_or_uuid);
	bool deleteProduct(std::string id_or_uuid);
	bool postProduct(std::string id_or_uuid);
};

#endif