#include "vtspch.h"
#include "s3_file_handler.h"

extern ConfigurationManager g_config;

bool S3::put_object_s3(const std::string& filename, std::stringstream& image_data) {
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

	Aws::S3::S3Client s3_client(credentials, client_config);


	Aws::S3::Model::PutObjectRequest request;
	request.SetBucket(g_config [ "AWS_S3_BUCKET_NAME" ].c_str());
	request.SetKey(filename.c_str());


	std::shared_ptr<Aws::IOStream> input_data =
		Aws::MakeShared<Aws::IOStream>("s3_allocation_tag",
									   image_data.rdbuf());

	request.SetBody(input_data);

	Aws::S3::Model::PutObjectOutcome outcome = s3_client.PutObject(request);

	if (outcome.IsSuccess()) {
		return true;
	} else {
		std::cout << "Error: PutObject: " <<
			outcome.GetError().GetMessage() << std::endl;
		return false;
	}
}

bool S3::get_object_s3(const std::string& filename, std::stringstream& stream) {
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

	Aws::S3::S3Client s3_client(credentials, client_config);

	Aws::S3::Model::GetObjectRequest object_request;
	object_request.SetBucket(g_config [ "AWS_S3_BUCKET_NAME" ].c_str());
	object_request.SetKey(filename.c_str());

	Aws::S3::Model::GetObjectOutcome get_object_outcome = s3_client.GetObject(object_request);

	if (get_object_outcome.IsSuccess()) {

		auto& file = get_object_outcome.GetResultWithOwnership().GetBody();

		stream << file.rdbuf();
		return true;
	} else {
		const auto& err = get_object_outcome.GetError();
		std::cout << "Error: GetObject: " <<
			err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
		return false;
	}
}

bool S3::delete_object_s3(const std::string& filename) {
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

	Aws::S3::S3Client s3_client(credentials, client_config);

	Aws::S3::Model::DeleteObjectRequest request;

	request.WithKey(filename.c_str()).WithBucket(g_config [ "AWS_S3_BUCKET_NAME" ].c_str());

	Aws::S3::Model::DeleteObjectOutcome outcome = s3_client.DeleteObject(request);

	if (!outcome.IsSuccess()) {
		auto err = outcome.GetError();
		std::cout << "Error: DeleteObject: " <<
			err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
		return false;
	} else {
		return true;
	}
}
