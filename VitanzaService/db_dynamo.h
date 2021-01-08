/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef DYNAMO_H
#define DYNAMO_H

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
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
#include <aws/dynamodb/model/ScanRequest.h>
#include <aws/dynamodb/model/ScanResult.h>

#include "config.h"

class DynamoDB {
public:
	static void get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, nlohmann::json& result_out);
	static bool update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
	static bool new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
	static bool delete_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value);
	static std::string scan_table_items_dynamo(const Aws::String& table_name);
	static std::string scan_table_items_filer_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& conditions_and_values);
	static std::string query_index(const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match);

private:
	static void client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config);
	static std::string dynamo_result_to_json_string(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result);
	static nlohmann::json parse_type(Aws::DynamoDB::Model::AttributeValue attr);
};
#endif
