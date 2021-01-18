/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#define _WIN32_WINNT 0x0a00
#endif

#include <httplib.h>

#include "config.h"
#include "logger.h"

#include "Auth.h"
#include "auth_wrapper.h"

#include "client_wrapper.h"
#include "chemical_order_wrapper.h"
#include "chemical_orderDetail_wrapper.h"
#include "chemical_product_wrapper.h"
#include "file_wrapper.h"

#include "Entities.h"

#include <aws/core/Aws.h>
#include <aws/core/VersionConfig.h>

#include "s3_file_handler.h"
#include "db_dynamo.h"

int main(int argc, char* argv[]);
void register_handlers(httplib::Server& svr);
void register_handlers_v2(httplib::Server& svr);
void set_response_headers(httplib::Response& res);

#endif