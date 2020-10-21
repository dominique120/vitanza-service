#ifndef ORDERDETAIL_WRAPPER_H
#define ORDERDETAIL_WRAPPER_H

#include "nlohmann/json.hpp"
#include "OrderDetail.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	static nlohmann::json get_order_detail(const std::string& id_or_uuid);
	static bool put_order_detail(const std::string& id_or_uuid);
	static bool delete_order_detail(const std::string& id_or_uuid);
	static bool post_order_detail(const std::string& id_or_uuid);
};

#endif