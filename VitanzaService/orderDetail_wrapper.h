#ifndef ORDERDETAIL_WRAPPER_H
#define ORDERDETAIL_WRAPPER_H

#include "nlohmann/json.hpp"
#include "OrderDetail.h"
#include "db_dynamo.h"

class Client_wrapper {
public:
	nlohmann::json getOrderDetail(const std::string& id_or_uuid);
	bool putOrderDetail(std::string id_or_uuid);
	bool deleteOrderDetail(std::string id_or_uuid);
	bool postOrderDetail(std::string id_or_uuid);
};

#endif