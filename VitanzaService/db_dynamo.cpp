#include "db_dynamo.h"


std::map<std::string, std::string> DynamoDB::getItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue) {
	std::map<std::string, std::string> newMap;

	Aws::Client::ClientConfiguration clientConfig;

	Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);
	Aws::DynamoDB::Model::GetItemRequest req;

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamoClient.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

	// Set up the request
	req.SetTableName(tableName); // table name
	Aws::DynamoDB::Model::AttributeValue hashKey;
	hashKey.SetS(keyValue);//primary key value to look for
	req.AddKey(keyName, hashKey); //primary key column name

	// Retrieve the item's fields and values
	const Aws::DynamoDB::Model::GetItemOutcome& result = dynamoClient.GetItem(req);
	if (result.IsSuccess()) {
		// Reference the retrieved fields/values
		const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& item = result.GetResult().GetItem();
		if (item.size() > 0) {
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
				newMap.insert(p);
			}
		} else {
			std::cout << "No item found with the key " << keyName << std::endl;
		}
	} else {
		std::cout << "Failed to get item: " << result.GetError().GetMessage();
	}
	return newMap;
}


bool DynamoDB::updateItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue, const nlohmann::json& json_req) {

	Aws::Client::ClientConfiguration clientConfig;
	Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamoClient.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

	// *** Define UpdateItem request arguments
	// Define TableName argument
	Aws::DynamoDB::Model::UpdateItemRequest request;
	request.SetTableName(tableName);

	// Define KeyName argument
	Aws::DynamoDB::Model::AttributeValue attribValue;
	attribValue.SetS(keyValue);
	request.AddKey(keyName, attribValue);


	// map json request to an std::map
	std::map<std::string, std::string> json_map = json_req;

	// TODO: I would like to make this mode dynamic instead of manually keeping all the tables here
	if (tableName == "clients") {
		json_map.erase("ClientId_uuid");
	} else if (tableName == "orders") {
		json_map.erase("OrderId_uuid");
	} else if (tableName == "orderdetails") {
		json_map.erase("OrderDetailId_uuid");
	} else if (tableName == "products") {
		json_map.erase("ProductId_uuid");
	} else {
		std::cout << "Unkouwn table!" << std::endl;
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
	Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> expressionAttributeValues;

	for (const auto& i : json_map) {
		Aws::DynamoDB::Model::AttributeValue attributeUpdatedValue;
		attributeUpdatedValue.SetS(i.second.c_str());//need to pass here the value to be updated
		Aws::OStringStream ostr;
		ostr << ":" << i.first;
		expressionAttributeValues [ ostr.str() ] = attributeUpdatedValue;
	}
	request.SetExpressionAttributeValues(expressionAttributeValues);


	// Update the item
	const Aws::DynamoDB::Model::UpdateItemOutcome& result = dynamoClient.UpdateItem(request);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}

	return true;
}

bool DynamoDB::newItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue, const nlohmann::json& json_req) {

	Aws::Client::ClientConfiguration clientConfig;
	Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamoClient.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

	std::map<Aws::String, Aws::String> items = json_req;

	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(tableName);

	for (const auto& i : items) {
		Aws::DynamoDB::Model::AttributeValue atval;
		atval.SetS(i.second);
		pir.AddItem(i.first, atval);
	}

	const Aws::DynamoDB::Model::PutItemOutcome result = dynamoClient.PutItem(pir);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}
	return true;
}

bool DynamoDB::deleteItemDynamo(const Aws::String& tableName, const Aws::String& keyName, const Aws::String& keyValue) {
	Aws::Client::ClientConfiguration clientConfig;
	Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

#ifdef _DYNAMO_LOCAL
	const Aws::String endpoint("http://localhost:8000");
	dynamoClient.OverrideEndpoint(endpoint);
#endif // _DYNAMO_LOCAL

	Aws::DynamoDB::Model::DeleteItemRequest req;

	Aws::DynamoDB::Model::AttributeValue hashKey;
	hashKey.SetS(keyValue);
	req.AddKey(keyName, hashKey);
	req.SetTableName(tableName);

	const Aws::DynamoDB::Model::DeleteItemOutcome& result = dynamoClient.DeleteItem(req);
	if (result.IsSuccess()) {
		return true;
	} else {
		std::cout << "Failed to delete item: " << result.GetError().GetMessage();
		return false;
	}
}