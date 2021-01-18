/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#pragma once
#ifndef REQUEST_HANDLERS_H
#define REQUEST_HANDLERS_H

#include "config.h"
#include <httplib.h>
#include "Entities.h"
#include "Auth.h"
#include "auth_wrapper.h"

class RequestHandlers {
public:
	static void set_client_handlers(httplib::Server& svr);
	static void set_product_handlers(httplib::Server& svr);
	static void set_order_handlers(httplib::Server& svr);
	static void set_order_detail_handlers(httplib::Server& svr);
	static void set_filter_installation_handlers(httplib::Server& svr);
	static void set_filter_change_handlers(httplib::Server& svr);
	static void set_note_handlers(httplib::Server& svr);
	static void set_authentication_handlers(httplib::Server& svr);

private:
	static void set_response_headers(httplib::Response& res);
};
#endif

