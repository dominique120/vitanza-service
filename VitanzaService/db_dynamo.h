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

	std::map<std::string, std::string> getItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue);
	
	bool updateItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue, const nlohmann::json& json_req);
	bool newItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue, const nlohmann::json& json_req);

	bool deleteItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue);


};
#endif