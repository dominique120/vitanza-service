#pragma once
#ifndef S3_FILE_HANDLER_H
#define S3_FILE_HANDLER_H

#if defined(S3_FILESTORE)
#include "config.h"

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/S3Errors.h>

class S3 {
public:
	static bool put_object_s3(const std::string& filename, std::stringstream& image_data, const bool& set_public);
	static bool get_object_s3(const std::string& filename, std::stringstream& stream);
	static bool delete_object_s3(const std::string& filename);
};
#endif

#endif
