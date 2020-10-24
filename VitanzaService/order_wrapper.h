#ifndef ORDER_WRAPPER_H
#define ORDER_WRAPPER_H

#include "nlohmann/json.hpp"
#include "Order.h"
#include "db_dynamo.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Order_wrapper {
public:
	static std::string get_order(const std::string& id_or_uuid);
	static bool delete_order(const std::string& id_or_uuid);
	static bool new_order(const std::string& request_body);
	static bool update_order(const std::string& id_or_uuid, const std::string& request_body);
};

#endif