/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef AWS_UTIL_H
#define AWS_UTIL_H
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include "aws/core/client/ClientConfiguration.h"
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "config.h"

class AWSUtil {
public:
	static void client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config);
	static std::string dynamo_result_to_string(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result);
};

#endif