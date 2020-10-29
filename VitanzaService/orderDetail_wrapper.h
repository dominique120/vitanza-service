#ifndef ORDERDETAIL_WRAPPER_H
#define ORDERDETAIL_WRAPPER_H

#include "OrderDetail.h"
#include "db_dynamo.h"


class OrderDetail_wrapper {
public:
	static std::string get_order_detail(const std::string& id_or_uuid);
	static bool delete_order_detail(const std::string& id_or_uuid);
	static bool new_order_detail(const std::string& request_body);
	static bool update_order_detail(const std::string& id_or_uuid, const std::string& request_body);
};

#endif