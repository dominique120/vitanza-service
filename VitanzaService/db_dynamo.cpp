/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "db_dynamo.h"

extern ConfigurationManager g_config;

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

void DynamoDB::get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, nlohmann::json& result_out) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	Aws::DynamoDB::Model::GetItemRequest req;
	// Set up the request
	req.SetTableName(table_name); // table name
	Aws::DynamoDB::Model::AttributeValue hash_key;
	hash_key.SetS(key_value);//primary key value to look for
	req.AddKey(key_name, hash_key); //primary key column name

	// Retrieve the item's fields and values
	const Aws::DynamoDB::Model::GetItemOutcome& result = dynamo_client.GetItem(req);
	if (result.IsSuccess()) {
		// Reference the retrieved fields/values
		const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& item = result.GetResult().GetItem();
		if (!item.empty()) {
			// Output each retrieved field and its value			
			for (const auto& i : item) {
				result_out[i.first.c_str()] = parse_type(i.second);
			}			
		}
		else {
			std::cout << "No item found with the key " << key_name << std::endl;
		}
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

	// *** Define UpdateItem request arguments
	// Define TableName argument
	Aws::DynamoDB::Model::UpdateItemRequest request;
	request.SetTableName(table_name);

	// Define KeyName argument
	Aws::DynamoDB::Model::AttributeValue attrib_value;
	attrib_value.SetS(key_value);
	request.AddKey(key_name, attrib_value);

	nlohmann::json j = nlohmann::json::parse(request_body);

	// map json request to an std::map
	std::map<std::string, std::string> json_map = j;

	json_map.erase(key_name.c_str());

	// build SET expression
	Aws::OStringStream ss;
	ss << "SET ";
	size_t counter = 0;
	for (const auto& i : json_map) {
		ss << i.first << " = :" << i.first;
		++counter;
		if (counter != json_map.size()) {
			ss << ", ";
		}
	}

	// Construct the SET update expression argument
	Aws::String update_expression(ss.str());
	request.SetUpdateExpression(update_expression);

	// Construct attribute value argument
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> expression_attribute_values;

	for (const auto& i : json_map) {
		Aws::DynamoDB::Model::AttributeValue attribute_updated_value;
		// TODO: change this to get they type of the item instead of manually filtering keys
		if (i.first == "Paid" || i.first == "Delivered" || i.first == "Stock" || i.first == "Price") {
			attribute_updated_value.SetN(i.second.c_str());//need to pass here the value to be updated
		}
		else {
			attribute_updated_value.SetS(i.second.c_str());//need to pass here the value to be updated
		}
		Aws::OStringStream ostr;
		ostr << ":" << i.first.c_str();
		expression_attribute_values[ostr.str()] = attribute_updated_value;
	}
	request.SetExpressionAttributeValues(expression_attribute_values);

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

	nlohmann::json j = nlohmann::json::parse(request_body);

	std::map<std::string, std::string> items = j;

	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(table_name);

	// Add body
	for (const auto& i : items) {
		Aws::DynamoDB::Model::AttributeValue attribute_value;
		// TODO: change this to get they type of the item instead of manually filtering keys
		if (i.first == "Paid" || i.first == "Delivered" || i.first == "Stock" || i.first == "Price") {
			attribute_value.SetN(i.second.c_str());
		}
		else {
			attribute_value.SetS(i.second.c_str());
		}
		pir.AddItem(i.first.c_str(), attribute_value);
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

std::string DynamoDB::query_index(const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;

	DynamoDB::client_config(credentials, client_config);

	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	const Aws::String query_expression(partition_key + " = :" + partition_key);
	//std::cout << query_expression.c_str() << std::endl;

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
		return "";
	}

	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> query_result = result.GetResult().GetItems();

	return DynamoDB::dynamo_result_to_json_string(query_result);
}

std::string DynamoDB::scan_table_items_dynamo(const Aws::String& table_name) {
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
		return "";
	}

	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> query_result = result.GetResult().GetItems();

	return DynamoDB::dynamo_result_to_json_string(query_result);
}

// Currently only tests for equality
std::string DynamoDB::scan_table_items_filer_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& conditions_and_values) {
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
		return "";
	}

	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> query_result = result.GetResult().GetItems();

	return DynamoDB::dynamo_result_to_json_string(query_result);
}

void DynamoDB::client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config) {
	aws_credentials.SetAWSAccessKeyId(Aws::String(g_config.AWS_ACCESS_KEY().c_str()));
	aws_credentials.SetAWSSecretKey(Aws::String(g_config.AWS_SECRET_KEY().c_str()));

	if (g_config.AWS_USE_SESSION_TOKEN()) {
		aws_credentials.SetSessionToken(Aws::String(g_config.AWS_SESSION_TOKEN().c_str()));
	}

	client_config.region = g_config.AWS_REGION().c_str();
}

std::string DynamoDB::dynamo_result_to_json_string(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result) {
	// convert to json
	// TODO: I absolutely don't like the way I use an inner counter and an outer counter. I'd like to replace that.
	// Either way it "seems" to work
	size_t inner_cntr = 0;
	size_t outer_cntr = 0;
	Aws::OStringStream j_ss;
	dynamo_result.empty() ? j_ss << "" : j_ss << "[";
	for (size_t i = 0; i < dynamo_result.size(); ++i) {
		j_ss << "{";
		for (const auto& j : dynamo_result.at(i)) {
			j_ss << "\"" << j.first << "\" : \"";
			// TODO: Could move this to some sort of recursive function to deal with nested objects
			if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
				j_ss << j.second.GetS();
			}
			else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
				j_ss << j.second.GetN();
			}
			else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
				j_ss << j.second.GetBool();
			}
			else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NULLVALUE) {
				j_ss << j.second.GetNull();
			}
			++inner_cntr;
			inner_cntr != dynamo_result.at(i).size() ? j_ss << "\", \n" : j_ss << "\" \n";
		}
		inner_cntr = 0;
		++outer_cntr;
		outer_cntr != dynamo_result.size() ? j_ss << "}, " : j_ss << "}";
	}
	dynamo_result.empty() ? j_ss << "" : j_ss << "]";

	return j_ss.str().c_str();
}
