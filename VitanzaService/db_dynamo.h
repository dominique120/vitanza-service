#ifndef DYNAMO_H
#define DYNAMO_H

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h> 
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/QueryResult.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <aws/dynamodb/model/UpdateItemResult.h>
#include <aws/dynamodb/model/DeleteItemResult.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>


#include "nlohmann/json.hpp"

class DynamoDB {
public:

	static std::map<std::string, std::string> get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value);

	static bool update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
	static bool new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);

	static bool delete_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value);
};
#endif