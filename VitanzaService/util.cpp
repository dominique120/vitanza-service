/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include "vtspch.h"
#include "util.h"
#include "config.h"

extern ConfigurationManager g_config;

std::unique_ptr<Aws::DynamoDB::DynamoDBClient> Util::ddb_vts_cli() {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;
	Util::client_config(credentials, client_config);
	auto cli =
		std::make_unique<Aws::DynamoDB::DynamoDBClient>(credentials, client_config);
	cli->OverrideEndpoint(g_config.AWS_DYNAMODB_ENDPOINT().c_str());
	return cli;
}

void Util::client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config) {
	aws_credentials.SetAWSAccessKeyId(Aws::String(g_config.AWS_ACCESS_KEY().c_str()));
	aws_credentials.SetAWSSecretKey(Aws::String(g_config.AWS_SECRET_KEY().c_str()));

	if (g_config.AWS_USE_SESSION_TOKEN()) {
		aws_credentials.SetSessionToken(Aws::String(g_config.AWS_SESSION_TOKEN().c_str()));
	}

	client_config.region = g_config.AWS_REGION().c_str();
}