#include "db_dynamo.h"

extern ConfigurationManager g_config;

std::map<std::string, std::string> DynamoDB::get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamo_client.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL
	
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
		std::cout << "Failed to get item: " << result.GetError().GetMessage();
	}
	return new_map;
}


bool DynamoDB::update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config [ "AWS_ACCESS_KEY" ].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config [ "AWS_SECRET_KEY" ].c_str()));
	
	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials ,client_config);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamo_client.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

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

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamo_client.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

	nlohmann::json j = nlohmann::json::parse(request_body);

	//for (auto i = j.begin(); i != j.end(); ++i) {
	//	std::cout << "key: " << i.key() << " - value: " << i.value() << std::endl;
	//}
	
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

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config [ "AWS_REGION" ].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamo_client.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

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