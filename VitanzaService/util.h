/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once
#ifndef UTIL_H
#define UTIL_H

#include "config.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
class Util {
public:
	static std::unique_ptr<Aws::DynamoDB::DynamoDBClient> ddb_vts_cli();
	static void client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config);
};
#endif