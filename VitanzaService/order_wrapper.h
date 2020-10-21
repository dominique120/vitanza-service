#ifndef ORDER_WRAPPER_H
#define ORDER_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Order.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	static nlohmann::json get_order(const std::string& id_or_uuid);
	static bool put_order(const std::string& id_or_uuid);
	static bool delete_order(const std::string& id_or_uuid);
	static bool post_order(const std::string& id_or_uuid);
};

#endif