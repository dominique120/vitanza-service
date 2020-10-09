#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#define _WIN32_WINNT 0x0a00
#endif




#ifdef _MSC_VER
#pragma warning(disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#endif
#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h> 
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>


#include <served/served.hpp>
#include <served/request_error.hpp>
#include <served/status.hpp>

#include <nlohmann\json.hpp>

#include "Client.h"
#include "Product.h"
#include "Order.h"
#include "OrderDetail.h"

int main();

#endif