#include "vtspch.h"
#include "db_dynamo.h"

#if defined(DB_DYNAMO)
extern ConfigurationManager g_config;

std::map<std::string, std::string> DynamoDB::get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
	dynamo_client.OverrideEndpoint(endpoint);
	
	std::map<std::string, std::string> new_map;
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
				std::pair<std::string, std::string> p;
				p.first = i.first.c_str();

				if (i.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
					p.second = i.second.GetS().c_str();
				} else if (i.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
					p.second = i.second.GetN().c_str();
				} else if (i.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
					p.second = i.second.GetBool();
				}
				new_map.insert(p);
			}
		} else {
			std::cout << "No item found with the key " << key_name << std::endl;
		}
	} else {
		std::cout << "Failed to get item: " << result.GetError().GetMessage() << std::endl;
	}
	return new_map;
}

bool DynamoDB::update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}
	
	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials ,client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
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

	// TODO: I would like to make this mode dynamic instead of manually keeping all the tables here
	if (table_name == "clients") {
		json_map.erase("ClientId_uuid");
	} else if (table_name == "orders") {
		json_map.erase("OrderId_uuid");
	} else if (table_name == "orderdetails") {
		json_map.erase("OrderDetailId_uuid");
	} else if (table_name == "products") {
		json_map.erase("ProductId_uuid");
	} else {
		std::cout << "Unknown table!" << std::endl;
	}

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
		attribute_updated_value.SetS(i.second.c_str());//need to pass here the value to be updated
		Aws::OStringStream ostr;
		ostr << ":" << i.first.c_str();
		expression_attribute_values [ ostr.str() ] = attribute_updated_value;
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
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	nlohmann::json j = nlohmann::json::parse(request_body);

	std::map<std::string, std::string> items = j;

	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(table_name);

	// Add body
	for (const auto& i : items) {
		Aws::DynamoDB::Model::AttributeValue attribute_value;
		attribute_value.SetS(i.second.c_str());
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
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	Aws::DynamoDB::Model::DeleteItemRequest req;

	Aws::DynamoDB::Model::AttributeValue hash_key;
	hash_key.SetS(key_value);
	req.AddKey(key_name, hash_key);
	req.SetTableName(table_name);

	const Aws::DynamoDB::Model::DeleteItemOutcome& result = dynamo_client.DeleteItem(req);
	if (result.IsSuccess()) {
		return true;
	} else {
		std::cout << "Failed to delete item: " << result.GetError().GetMessage();
		return false;
	}
}

// TODO: method not working for some reason, use SCAN for the moment
std::string DynamoDB::query_table_items_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& index_condition_map) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
	dynamo_client.OverrideEndpoint(endpoint);


	/* Build condition expression */
	Aws::OStringStream ss;
	size_t counter = 0;
	for (const auto& i : index_condition_map) {
		ss << i.first << " = :" << i.first;
		++counter;
		if (counter != index_condition_map.size()) {
			ss << " and ";
		}
	}
	Aws::String query_expression(ss.str());


	// Construct attribute value argument
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> expression_attribute_values;
	/* Add parameters to expression */
	for (const auto& i : index_condition_map) {
		Aws::DynamoDB::Model::AttributeValue attribute_query_value;
		attribute_query_value.SetS(i.second.c_str());//need to pass here the condition of the query
		Aws::OStringStream ostr;
		ostr << ":" << i.first.c_str();
		expression_attribute_values [ ostr.str() ] = attribute_query_value;
	}

	Aws::DynamoDB::Model::QueryRequest query_request;
	query_request.SetTableName(table_name);
	query_request.SetKeyConditionExpression(query_expression);
	query_request.SetExpressionAttributeValues(expression_attribute_values);

	// run the query
	const Aws::DynamoDB::Model::QueryOutcome& result = dynamo_client.Query(query_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return "";
	}
	
	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> query_result = result.GetResult().GetItems();

	// convert to json
	size_t cntr = 0;
	Aws::OStringStream j_ss;
	j_ss << "[";
	for (size_t i = 0; i < query_result.size(); ++i) {
		j_ss << "{";
		for (auto j : query_result.at(i)) {
			j_ss << "\"" << j.first << "\" : \"";

			if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
				j_ss << j.second.GetS().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
				j_ss << j.second.GetN().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
				j_ss << j.second.GetBool();
			}
			j_ss << "\"";
		}
		if (cntr != query_result.size()) {
			j_ss << "}, ";
		} else {
			j_ss << "}";
		}	
	}
	j_ss << "]";
	Aws::String json_result_aws = j_ss.str();

	return json_result_aws.c_str();
}

std::string DynamoDB::scan_table_items_dynamo(const Aws::String& table_name) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
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

	// convert to json
	// TODO: I absolutely don't like the way I use an inner counter and an outer counter. I'd like to replace that.
	// Either way it "seems" to work
	size_t inner_cntr = 0;
	size_t outer_cntr = 0;
	Aws::OStringStream j_ss;
	j_ss << "[";
	for (size_t i = 0; i < query_result.size(); ++i) {
		j_ss << "{";
		for (auto &j : query_result.at(i)) {
			j_ss << "\"" << j.first << "\" : \"";

			if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
				j_ss << j.second.GetS().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
				j_ss << j.second.GetN().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
				j_ss << j.second.GetBool();
			}
			++inner_cntr;
			inner_cntr != query_result.at(i).size() ? j_ss << "\", \n" : j_ss << "\" \n";			
		}
		inner_cntr = 0;
		++outer_cntr;
		outer_cntr != query_result.size() ? j_ss << "}, " : j_ss << "}";
	}
	j_ss << "]";

	return j_ss.str().c_str();
}






// Currently only tests for equality
std::string DynamoDB::scan_table_items_filer_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& conditions_and_values) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	bool use_token;
	std::istringstream(g_config [ "AWS_USE_SESSION_TOKEN" ]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config [ "AWS_SESSION_TOKEN" ].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config [ "AWS_DYNAMODB_ENDPOINT" ].c_str());
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
		attribute_updated_value.SetN(std::stoi(i.second));//need to pass here the value to be updated
		Aws::OStringStream ostr;
		ostr << ":" << i.first.c_str();
		expression_attribute_values [ ostr.str() ] = attribute_updated_value;
	}
	scan_request.SetExpressionAttributeValues(expression_attribute_values);





	
	// run the scan
	const Aws::DynamoDB::Model::ScanOutcome& result = dynamo_client.Scan(scan_request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return "";
	}

	const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> query_result = result.GetResult().GetItems();

	// convert to json
	// TODO: I absolutely don't like the way I use an inner counter and an outer counter. I'd like to replace that.
	// Either way it "seems" to work
	size_t inner_cntr = 0;
	size_t outer_cntr = 0;
	Aws::OStringStream j_ss;
	j_ss << "[";
	for (size_t i = 0; i < query_result.size(); ++i) {
		j_ss << "{";
		for (auto& j : query_result.at(i)) {
			j_ss << "\"" << j.first << "\" : \"";

			if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
				j_ss << j.second.GetS().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
				j_ss << j.second.GetN().c_str();
			} else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
				j_ss << j.second.GetBool();
			}
			++inner_cntr;
			inner_cntr != query_result.at(i).size() ? j_ss << "\", \n" : j_ss << "\" \n";
		}
		inner_cntr = 0;
		++outer_cntr;
		outer_cntr != query_result.size() ? j_ss << "}, " : j_ss << "}";
	}
	j_ss << "]";

	return j_ss.str().c_str();
}



#endif
