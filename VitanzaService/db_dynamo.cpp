/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "db_dynamo.h"

extern ConfigurationManager g_config;

void DynamoDB::compose_type(Aws::DynamoDB::Model::AttributeValue& attr, const nlohmann::json& json) {
	for (const auto& item : json.items()) {
		if (item.value().is_number_integer()) {
			attr.SetN(std::to_string(item.value().get<long>()).c_str());
		}
		else if (item.value().is_number_float()) {
			attr.SetN(item.value().get<double>());
		}
		else if (item.value().is_string()) {
			attr.SetS(item.value().get<std::string>().c_str());
		}
		else if (item.value().is_boolean()) {
			attr.SetBool(item.value().get<bool>());
		}
		else if (item.value().is_array()) {
			Aws::Vector< std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> array;

			for (const auto& array_object : item.value().items()) {
				Aws::DynamoDB::Model::AttributeValue temp_attr;
				compose_object(temp_attr, array_object.value());
				array.push_back(Aws::MakeShared<Aws::DynamoDB::Model::AttributeValue>("", temp_attr));
			}
			attr.SetL(array);
		}
		else if (item.value().is_object()) {
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
		}
		else if (item.value().is_null()) {
			attr.SetNull(true);
		}
	}
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

nlohmann::json DynamoDB::parse_type(Aws::DynamoDB::Model::AttributeValue attr) {
	if (attr.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
		return attr.GetS();
	}
	else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
		return attr.GetN();
	}
	else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
		return attr.GetBool();
	}
	else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::ATTRIBUTE_MAP) {
		nlohmann::json json;
		auto type = attr.GetM();
		for (const auto& element : type) {
			json[element.first.c_str()] = parse_type(*element.second);
		}
		return json;
	}
	else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::ATTRIBUTE_LIST) {
		nlohmann::json json = nlohmann::json::array();
		auto type = attr.GetL();
		for (const auto& element : type) {
			nlohmann::json obj = parse_type(*element);
			json.push_back(obj);
		}
		return json;
	}
	else if (attr.GetType() == Aws::DynamoDB::Model::ValueType::NULLVALUE) {
		return nullptr;
	}
	else {
		return nullptr;
	}
}

void DynamoDB::parse_object(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& dynamo_result, nlohmann::json& json_out) {
	for (const auto& element : dynamo_result) {
		json_out[element.first.c_str()] = parse_type(element.second);
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

void DynamoDB::get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, nlohmann::json& result_out) {
	/* Config */
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;
	DynamoDB::client_config(credentials, client_config);
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);
	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);
	/* Config */

	Aws::DynamoDB::Model::GetItemRequest req;

	// Set up the request
	req.SetTableName(table_name); // table name
	Aws::DynamoDB::Model::AttributeValue hash_key;
	hash_key.SetS(key_value);//primary key value to look for
	req.AddKey(key_name, hash_key); //primary key column name

	// Retrieve the item's fields and values
	const Aws::DynamoDB::Model::GetItemOutcome& result = dynamo_client.GetItem(req);
	if (result.IsSuccess()) {
		parse_object(result.GetResult().GetItem(), result_out);
	}
	else {
		std::cout << "Failed to get item: " << result.GetError().GetMessage() << std::endl;
	}
}

bool DynamoDB::update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	// Define TableName argument
	Aws::DynamoDB::Model::UpdateItemRequest request;
	request.SetTableName(table_name);

	// Define KeyName argument
	Aws::DynamoDB::Model::AttributeValue attrib_value;
	attrib_value.SetS(key_value);
	request.AddKey(key_name, attrib_value);
	nlohmann::json j;
	try {
		j = nlohmann::json::parse(request_body);
		j.erase(key_name.c_str());
	}
	catch (nlohmann::json::exception) {
		return false;
	}

	// set expression for SET
	request.SetUpdateExpression(build_operation_expression(j, "SET"));

	// Construct attribute value argument
	request.SetExpressionAttributeValues(build_operation_values(j));

	// Update the item
	const Aws::DynamoDB::Model::UpdateItemOutcome& result = dynamo_client.UpdateItem(request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}

	return true;
}

bool DynamoDB::new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

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

	const Aws::DynamoDB::Model::PutItemOutcome result = dynamo_client.PutItem(pir);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}
	return true;
}

bool DynamoDB::delete_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	Aws::DynamoDB::Model::DeleteItemRequest req;

	Aws::DynamoDB::Model::AttributeValue hash_key;
	hash_key.SetS(key_value);
	req.AddKey(key_name, hash_key);
	req.SetTableName(table_name);

	const Aws::DynamoDB::Model::DeleteItemOutcome& result = dynamo_client.DeleteItem(req);
	if (result.IsSuccess()) {
		return true;
	}
	else {
		std::cout << "Failed to delete item: " << result.GetError().GetMessage();
		return false;
	}
}

void DynamoDB::query_with_expression(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& expression, const nlohmann::json& expression_values, nlohmann::json& result_out) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);
		
	Aws::DynamoDB::Model::QueryRequest query_request;
	query_request.SetTableName(table_name);
	query_request.SetIndexName(key_name);

	query_request.SetKeyConditionExpression(expression);
	query_request.SetExpressionAttributeValues(build_operation_values(expression_values));

	// run the query
	const Aws::DynamoDB::Model::QueryOutcome& result = dynamo_client.Query(query_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

void DynamoDB::query_index(const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match, nlohmann::json& result_out) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

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
	const Aws::DynamoDB::Model::QueryOutcome& result = dynamo_client.Query(query_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

void DynamoDB::scan_table_items_dynamo(const Aws::String& table_name, nlohmann::json& result_out) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	Aws::DynamoDB::Model::ScanRequest scan_request;
	scan_request.SetTableName(table_name);

	// run the query
	const Aws::DynamoDB::Model::ScanOutcome& result = dynamo_client.Scan(scan_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

// Currently only tests for equality
void DynamoDB::scan_table_items_filer_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& conditions_and_values, nlohmann::json& result_out) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	Aws::DynamoDB::Model::ScanRequest scan_request;
	scan_request.SetTableName(table_name);

	// build expression
	Aws::OStringStream ss;
	size_t counter = 0;
	for (const auto& i : conditions_and_values) {
		ss << i.first << " = :" << i.first;
		++counter;
		if (counter != conditions_and_values.size()) {
			ss << " or ";
		}
	}
	//std::cout << ss.str() << std::endl;

	// Construct the expression argument
	Aws::String scan_expression(ss.str());
	scan_request.SetFilterExpression(scan_expression);

	// Construct attribute value argument
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> expression_attribute_values;

	// TODO: Need to filter for the type of the conditional to use
	for (const auto& i : conditions_and_values) {
		Aws::DynamoDB::Model::AttributeValue attribute_updated_value;
		attribute_updated_value.SetN(i.second.c_str());//need to pass here the value to be updated
		Aws::OStringStream ostr;
		ostr << ":" << i.first.c_str();
		expression_attribute_values[ostr.str()] = attribute_updated_value;
	}
	scan_request.SetExpressionAttributeValues(expression_attribute_values);

	// run the scan
	const Aws::DynamoDB::Model::ScanOutcome& result = dynamo_client.Scan(scan_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
	}

	DynamoDB::parse_collection(result.GetResult().GetItems(), result_out);
}

void DynamoDB::client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config) {
	aws_credentials.SetAWSAccessKeyId(Aws::String(g_config.AWS_ACCESS_KEY().c_str()));
	aws_credentials.SetAWSSecretKey(Aws::String(g_config.AWS_SECRET_KEY().c_str()));

	if (g_config.AWS_USE_SESSION_TOKEN()) {
		aws_credentials.SetSessionToken(Aws::String(g_config.AWS_SESSION_TOKEN().c_str()));
	}

	client_config.region = g_config.AWS_REGION().c_str();
}

