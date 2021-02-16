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

	struct CompositePK {
		Aws::String pk_name;
		Aws::String pk_value;
		Aws::String sk_name;
		Aws::String sk_value;
	};
	
	// called in create_user
	static bool new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
		
	// called in validate_user
	static void query_index(const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match, nlohmann::json& result_out);

	static void query_with_expression(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& expression, const nlohmann::json& expression_values, nlohmann::json& result_out);
	static void get_item_composite(const Aws::String& table_name, const CompositePK& primary_key, nlohmann::json& result_out);
	static bool put_item(const nlohmann::json& request, const std::string& table);
	static bool update_item_composite(const nlohmann::json& request, const std::string& table, const CompositePK& primary_key);
	static bool delete_item_composite(const Aws::String& table_name, const CompositePK& primary_key);

private:
	static void client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config);
	static void parse_collection(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result, nlohmann::json& json_out);
	static void parse_object(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& dynamo_result, nlohmann::json& json_out);
	static nlohmann::json parse_type(Aws::DynamoDB::Model::AttributeValue attr);
	static void compose_type(Aws::DynamoDB::Model::AttributeValue& attr, const nlohmann::json& json);
	static void compose_object(Aws::DynamoDB::Model::AttributeValue& attr, const nlohmann::json& json);
	static Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> build_operation_values(const nlohmann::json& json);
	static Aws::String build_operation_expression(const nlohmann::json& json, const std::string& operation);
	static std::unique_ptr<Aws::DynamoDB::DynamoDBClient> make_client();
};
#endif
