#ifndef ORDERDETAIL_WRAPPER_H
#define ORDERDETAIL_WRAPPER_H

#include "nlohmann/json.hpp"
#include "OrderDetail.h"
#include "db_dynamo.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class OrderDetail_wrapper {
public:
	static std::string get_order_detail(const std::string& id_or_uuid);
	static bool delete_order_detail(const std::string& id_or_uuid);
	static bool new_order_detail(const std::string& request_body);
	static bool update_order_detail(const std::string& id_or_uuid, const std::string& request_body);
};

#endif