#ifndef ORDER_WRAPPER_H
#define ORDER_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Order.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	nlohmann::json getOrder(const std::string& id_or_uuid);
	bool putOrder(std::string id_or_uuid);
	bool deleteOrder(std::string id_or_uuid);
	bool postOrder(std::string id_or_uuid);
};

#endif