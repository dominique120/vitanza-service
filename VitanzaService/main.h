/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#define _WIN32_WINNT 0x0a00
#endif

#if !defined(DB_DYNAMO) && !defined(DB_MYSQL)
#error "You must define either DB_DYNAMO or DB_MYSQL, otherwise you will not have database coonectivity."
#endif

#if defined(DB_DYNAMO) && defined(DB_MYSQL)
#error "You can only specify a single database."
#endif

#ifdef _MSC_VER
#pragma warning(disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#endif

#include <httplib.h>

#include "config.h"
#include "logger.h"

#include "Auth.h"
#include "auth_wrapper.h"

#include "Client.h"
#include "Product.h"
#include "Order.h"
#include "OrderDetail.h"

#include "client_wrapper.h"
#include "order_wrapper.h"
#include "orderDetail_wrapper.h"
#include "product_wrapper.h"
#include "file_wrapper.h"

#include <aws/core/Aws.h>
#include <aws/core/VersionConfig.h>

#include "s3_file_handler.h"
#include "db_dynamo.h"
#include "sqs_handler.h"

int main(int argc, char* argv[]);
void register_handlers(httplib::Server& svr);

#endif