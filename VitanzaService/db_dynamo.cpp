/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "db_dynamo.h"

extern ConfigurationManager g_config;


// utilities
std::unique_ptr<Aws::DynamoDB::DynamoDBClient> DynamoDB::make_default_client() {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;
	DynamoDB::client_config(credentials, client_config);
	auto cli =
		std::make_unique<Aws::DynamoDB::DynamoDBClient>(credentials, client_config);	
	cli->OverrideEndpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	return cli;
}

void DynamoDB::client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config) {
	aws_credentials.SetAWSAccessKeyId(Aws::String(g_config.AWS_ACCESS_KEY().c_str()));
	aws_credentials.SetAWSSecretKey(Aws::String(g_config.AWS_SECRET_KEY().c_str()));

	if (g_config.AWS_USE_SESSION_TOKEN()) {
		aws_credentials.SetSessionToken(Aws::String(g_config.AWS_SESSION_TOKEN().c_str()));
	}

	client_config.region = g_config.AWS_REGION().c_str();
}

Aws::String DynamoDB::build_operation_expression(const nlohmann::json& json, const std::string& operation) {
	std::stringstream ss;
	ss << operation << " ";
	for (const auto& item : json.items()) {
		ss << item.key() << " = :" << item.key() << ", ";
	}
	Aws::String expr = ss.str().c_str();
	expr.pop_back();
	expr.pop_back();
	return expr;
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> DynamoDB::build_operation_values(const nlohmann::json& json) {
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> object;
	for (const auto& item : json.items()) {
		std::pair<Aws::String, Aws::DynamoDB::Model::AttributeValue> pair;

		Aws::DynamoDB::Model::AttributeValue temp_attr;

		pair.first = ":" + item.key();
		compose_type(temp_attr, item.value());
		pair.second = temp_attr;

		object.insert(pair);
	}
	return object;
}


// type parsing
void DynamoDB::compose_object(Aws::DynamoDB::Model::AttributeValue& attr, const nlohmann::json& json) {
	Aws::Map<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> object;
	for (const auto& item : json.items()) {
		std::pair<Aws::String, std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> pair;

		Aws::DynamoDB::Model::AttributeValue temp_attr;

		pair.first = item.key();
		compose_type(temp_attr, item.value());
		pair.second = Aws::MakeShared<Aws::DynamoDB::Model::AttributeValue>("", temp_attr);

		object.insert(pair);
	}
	attr.SetM(object);
}

void DynamoDB::parse_object(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& dynamo_result, nlohmann::json& json_out) {
	for (const auto& element : dynamo_result) {
		json_out[element.first.c_str()] = parse_type(element.second);
	}
}

nlohmann::json DynamoDB::parse_type(Aws::DynamoDB::Model::AttributeValue attr) {
	if (attr.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
		return attr.GetS();
	} else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
		return attr.GetN();
	} else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
		return attr.GetBool();
	} else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::ATTRIBUTE_MAP) {
		nlohmann::json json;
		auto type = attr.GetM();
		for (const auto& element : type) {
			json[element.first.c_str()] = parse_type(*element.second);
		}
		return json;
	} else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::ATTRIBUTE_LIST) {
		nlohmann::json json = nlohmann::json::array();
		auto type = attr.GetL();
		for (const auto& element : type) {
			nlohmann::json obj = parse_type(*element);
			json.push_back(obj);
		}
		return json;
	} else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::NULLVALUE) {
		return nullptr;
	} else {
		return nullptr;
	}
}

void DynamoDB::compose_type(Aws::DynamoDB::Model::AttributeValue& attr, const nlohmann::json& json) {
	for (const auto& item : json.items()) {
		if (item.value().is_number_integer()) {
			attr.SetN(std::to_string(item.value().get<long>()).c_str());
		} else if (item.value().is_number_float()) {
			attr.SetN(item.value().get<double>());
		} else if (item.value().is_string()) {
			attr.SetS(item.value().get<std::string>().c_str());
		} else if (item.value().is_boolean()) {
			attr.SetBool(item.value().get<bool>());
		} else if (item.value().is_array()) {
			Aws::Vector< std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> array;

			for (const auto& array_object : item.value().items()) {
				Aws::DynamoDB::Model::AttributeValue temp_attr;
				compose_object(temp_attr, array_object.value());
				array.push_back(Aws::MakeShared<Aws::DynamoDB::Model::AttributeValue>("", temp_attr));
			}
			attr.SetL(array);
		} else if (item.value().is_object()) {
			Aws::Map<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> object;
			for (const auto& nested_item : item.value().items()) {
				std::pair<Aws::String, std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> pair;

				Aws::DynamoDB::Model::AttributeValue temp_attr;

				pair.first = nested_item.key();
				compose_type(temp_attr, nested_item.value());
				pair.second = Aws::MakeShared<Aws::DynamoDB::Model::AttributeValue>("", temp_attr);

				object.insert(pair);
			}
			attr.SetM(object);
		} else if (item.value().is_null()) {
			attr.SetNull(true);
		}
	}
}

void DynamoDB::parse_collection(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result, nlohmann::json& json_out) {
	json_out = nlohmann::json::array();
	for (const auto& object : dynamo_result) {
		nlohmann::json obj;
		parse_object(object, obj);
		json_out.push_back(obj);
	}
}


// basic operations
bool DynamoDB::put_item(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const nlohmann::json& request, const std::string& table) {
	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(table.c_str());

	// Add body
	for (const auto& element : request.items()) {
		Aws::DynamoDB::Model::AttributeValue attribute_value;
		compose_type(attribute_value, element.value());
		pir.AddItem(element.key().c_str(), attribute_value);
	}

	const Aws::DynamoDB::Model::PutItemOutcome outcome = client->PutItem(pir);
	if (!outcome.IsSuccess()) {
		std::cout << outcome.GetError().GetMessage() << std::endl;
		return false;
	}
	return true;
}

void DynamoDB::get_item_composite(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const Aws::String& table_name, const CompositePK& primary_key, nlohmann::json& result_out) {
	Aws::DynamoDB::Model::GetItemRequest req;

	// Set up the request
	req.SetTableName(table_name); // table name

	// Setup the composite key for the GetItemRequest
	Aws::DynamoDB::Model::AttributeValue pk;
	pk.SetS(primary_key.pk_value);
	req.AddKey(primary_key.pk_name, pk);

	Aws::DynamoDB::Model::AttributeValue sk;
	sk.SetS(primary_key.sk_value);
	req.AddKey(primary_key.sk_name, sk);

	// Retrieve the item's fields and values
	const Aws::DynamoDB::Model::GetItemOutcome& result = client->GetItem(req);
	if (result.IsSuccess()) {
		parse_object(result.GetResult().GetItem(), result_out);
	} else {
		std::cout << "Failed to get item: " << result.GetError().GetMessage() << std::endl;
	}
}

bool DynamoDB::update_item_composite(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const nlohmann::json& request, const std::string& table, const CompositePK& primary_key) {
	// Define TableName argument
	Aws::DynamoDB::Model::UpdateItemRequest uir;
	uir.SetTableName(table.c_str());

	// Setup the composite key 
	Aws::DynamoDB::Model::AttributeValue pk;
	pk.SetS(primary_key.pk_value);
	uir.AddKey(primary_key.pk_name, pk);

	Aws::DynamoDB::Model::AttributeValue sk;
	sk.SetS(primary_key.sk_value);
	uir.AddKey(primary_key.sk_name, sk);

	// set expression for SET
	uir.SetUpdateExpression(build_operation_expression(request, "SET"));

	// Construct attribute value argument
	uir.SetExpressionAttributeValues(build_operation_values(request));

	// Update the item
	const Aws::DynamoDB::Model::UpdateItemOutcome& result = client->UpdateItem(uir);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}

	return true;
}

bool DynamoDB::delete_item_composite(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const Aws::String& table_name, const CompositePK& primary_key) {
	Aws::DynamoDB::Model::DeleteItemRequest req;

	// Setup the composite key 
	Aws::DynamoDB::Model::AttributeValue pk;
	pk.SetS(primary_key.pk_value);
	req.AddKey(primary_key.pk_name, pk);

	Aws::DynamoDB::Model::AttributeValue sk;
	sk.SetS(primary_key.sk_value);
	req.AddKey(primary_key.sk_name, sk);

	// Set table name
	req.SetTableName(table_name);

	const Aws::DynamoDB::Model::DeleteItemOutcome& result = client->DeleteItem(req);
	if (result.IsSuccess()) {
		return true;
	} else {
		std::cout << "Failed to delete item: " << result.GetError().GetMessage();
		return false;
	}
}

bool DynamoDB::new_item_dynamo(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	nlohmann::json item = nlohmann::json::parse(request_body);

	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(table_name);

	// Add body
	for (const auto& element : item.items()) {
		Aws::DynamoDB::Model::AttributeValue attribute_value;
		compose_type(attribute_value, element);
		pir.AddItem(element.key().c_str(), attribute_value);
	}

	// Add key
	Aws::DynamoDB::Model::AttributeValue attribute_value;
	attribute_value.SetS(key_value);
	pir.AddItem(key_name, attribute_value);

	const Aws::DynamoDB::Model::PutItemOutcome result = client->PutItem(pir);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}
	return true;
}


// queries
void DynamoDB::query_with_expression(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const Aws::String& table_name, const Aws::String& key_name, const Aws::String& expression, const nlohmann::json& expression_values, nlohmann::json& result_out) {
	Aws::DynamoDB::Model::QueryRequest query_request;
	query_request.SetTableName(table_name);
	if (!key_name.empty()) {
		query_request.SetIndexName(key_name);
	}

	query_request.SetKeyConditionExpression(expression);
	query_request.SetExpressionAttributeValues(build_operation_values(expression_values));

	// run the query
	const Aws::DynamoDB::Model::QueryOutcome& result = client->Query(query_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

void DynamoDB::query_index(std::unique_ptr<Aws::DynamoDB::DynamoDBClient> client, const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match, nlohmann::json& result_out) {
	const Aws::String query_expression(partition_key + " = :" + partition_key);

	// Construct attribute value argument
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> expression_attribute_values;
	Aws::DynamoDB::Model::AttributeValue attribute_query_value;
	attribute_query_value.SetS(match.c_str());
	expression_attribute_values[":" + partition_key] = attribute_query_value;

	Aws::DynamoDB::Model::QueryRequest query_request;
	query_request.SetTableName(table_name);
	query_request.SetIndexName(partition_key);

	query_request.SetKeyConditionExpression(query_expression);
	query_request.SetExpressionAttributeValues(expression_attribute_values);

	// run the query
	const Aws::DynamoDB::Model::QueryOutcome& result = client->Query(query_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

