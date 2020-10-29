#ifndef ORDER_WRAPPER_H
#define ORDER_WRAPPER_H

#include "Order.h"
#include "db_dynamo.h"

class Order_wrapper {
public:
	static std::string get_order(const std::string& id_or_uuid);
	static bool delete_order(const std::string& id_or_uuid);
	static bool new_order(const std::string& request_body);
	static bool update_order(const std::string& id_or_uuid, const std::string& request_body);
	static std::string get_all_orders();
	static std::string get_outstanding_orders();
};

#endif