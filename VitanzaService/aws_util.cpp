/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "aws_util.h"

extern ConfigurationManager g_config;

void AWSUtil::client_config(Aws::Auth::AWSCredentials& aws_credentials, Aws::Client::ClientConfiguration& client_config) {
	aws_credentials.SetAWSAccessKeyId(Aws::String(g_config["AWS_ACCESS_KEY"].c_str()));
	aws_credentials.SetAWSSecretKey(Aws::String(g_config["AWS_SECRET_KEY"].c_str()));
	bool use_token;
	std::istringstream(g_config["AWS_USE_SESSION_TOKEN"]) >> std::boolalpha >> use_token;
	if (use_token) {
		aws_credentials.SetSessionToken(Aws::String(g_config["AWS_SESSION_TOKEN"].c_str()));
	}	

	client_config.region = g_config["AWS_REGION"].c_str();
}

std::string AWSUtil::dynamo_result_to_json(const Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>>& dynamo_result) {
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

			if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::STRING) {
				j_ss << j.second.GetS().c_str();
			}
			else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::NUMBER) {
				j_ss << j.second.GetN().c_str();
			}
			else if (j.second.GetType() == Aws::DynamoDB::Model::ValueType::BOOL) {
				j_ss << j.second.GetBool();
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
