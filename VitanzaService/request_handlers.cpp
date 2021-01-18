#include "vtspch.h"
#include "request_handlers.h"


extern ConfigurationManager g_config;

void RequestHandlers::set_response_headers(httplib::Response& res) {
	res.set_header("Content-type", "application/json");
	res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE, PUT");
	// Set port to 4200 for local angular development. Otherwise must use the domain name with the protocol.
	res.set_header("Access-Control-Allow-Origin", g_config.CORS_ORIGIN());
	res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
	res.set_header("Access-Control-Allow-Credentials", "true");
}

void RequestHandlers::set_client_handlers(httplib::Server& svr) {
	svr.Get((g_config.API_BASE_URL_V2() + "/client").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 1
		if (req.has_param("status")) {
			nlohmann::json result;
			Client::query_clients_by_status(req.get_param_value("status"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}

			// AP 2
		} else 	if (req.has_param("id")) {
			nlohmann::json result;
			Client::get_client(req.get_param_value("id"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		} else {
			res.status = 400;
		}
		});

	svr.Post((g_config.API_BASE_URL_V2() + "/client").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}


		});
}

void RequestHandlers::set_product_handlers(httplib::Server& svr) {
	svr.Get((g_config.API_BASE_URL_V2() + "/product").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 9
		if (req.has_param("productid") && req.has_param("category")) {
			nlohmann::json result;
			Product::get_product(req.get_param_value("category"), req.get_param_value("productid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		}

		// AP 10
		else if (req.has_param("category")) {
			nlohmann::json result;
			Product::get_current_stock(req.get_param_value("category"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		} else {
			res.status = 400;
		}
		});
}

void RequestHandlers::set_order_handlers(httplib::Server& svr) {

	svr.Get((g_config.API_BASE_URL_V2() + "/order").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 7
		if (req.has_param("orderid") && req.has_param("clientid")) {
			nlohmann::json result;
			Order::get_order(req.get_param_value("clientid"), req.get_param_value("orderid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		}

		// AP 3
		else if (req.has_param("clientid")) {
			nlohmann::json result;
			Order::query_orders_by_client(req.get_param_value("clientid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		}

		// AP 5
		else if (req.has_param("status")) {
			nlohmann::json result;
			Order::query_orders_by_status(req.get_param_value("status"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		} else {
			res.status = 400;
		}
		});
}

void RequestHandlers::set_order_detail_handlers(httplib::Server& svr) {
	svr.Get((g_config.API_BASE_URL_V2() + "/orderdetail").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 8
		if (req.has_param("orderid")) {
			nlohmann::json result;
			OrderDetail::get_order_details_by_order(req.get_param_value("orderid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		} else {
			res.status = 400;
		}
		});
}

void RequestHandlers::set_filter_installation_handlers(httplib::Server& svr) {

	svr.Get((g_config.API_BASE_URL_V2() + "/filter").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 6
		if (req.has_param("filterid") && req.has_param("clientid")) {
			nlohmann::json result;
			FilterInstallation::get_filter_installation(req.get_param_value("clientid"), req.get_param_value("filterid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		}

		// AP 4
		else if (req.has_param("clientid")) {
			nlohmann::json result;
			FilterInstallation::query_filter_installations_by_client(req.get_param_value("clientid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		} else {
			res.status = 400;
		}
		});
}

void RequestHandlers::set_filter_change_handlers(httplib::Server& svr) {
	svr.Get((g_config.API_BASE_URL_V2() + "/filterchange").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 13
		if (req.has_param("status") && req.has_param("start") && req.has_param("end")) {
			nlohmann::json result;
			FilterChange::get_changes_by_status_dates(req.get_param_value("status"), req.get_param_value("start"), req.get_param_value("end"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}
		}

		// AP 12
		else if (req.has_param("filterid")) {
			nlohmann::json result;
			FilterChange::get_changes_by_installation(req.get_param_value("filterid"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}

		} else {
			res.status = 400;
		}
		});
}

void RequestHandlers::set_note_handlers(httplib::Server& svr) {

	svr.Get((g_config.API_BASE_URL_V2() + "/note").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);

		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		// AP 11
		if (req.has_param("status")) {
			nlohmann::json result;
			Note::get_notes_by_status(req.get_param_value("status"), result);

			if (result.empty()) {
				res.status = 204;
			} else {
				res.status = 200;
				res.body = result.dump();
			}

		} else {
			res.status = 400;
		}
		});

}

void RequestHandlers::set_authentication_handlers(httplib::Server& svr) {
	svr.Options(R"(.*)", [&](const httplib::Request& req, httplib::Response& res) {
		set_response_headers(res);
		});

	svr.Post((g_config.API_BASE_URL() + "/config").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (g_config.reload(req.body)) {
			res.status = 201;
		} else {
			res.status = 400;
		}
		});

	svr.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
		res.status = 200;
		});

	/*----------------- Authentication -------------------------------*/
	// later on add support for creating new password, deleting user, etc
	svr.Post((g_config.API_BASE_URL() + "/auth/users").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}
		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);

		if (auth_wrapper::save_new_user(user.at("username"), user.at("password"))) {
			res.status = 201;
		} else {
			res.status = 400;
		}
		});

	svr.Post((g_config.API_BASE_URL() + "/auth/validateuser").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);

		if (auth_wrapper::authenticate(user.at("username"), user.at("password"))) {
			res.status = 200;
		} else {
			res.status = 403;
		}
		});

	svr.Post((g_config.API_BASE_URL() + "/auth").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);
		set_response_headers(res);
		if (auth_wrapper::authenticate(user.at("username"), user.at("password"))) {
			res.set_header("Content-type", "application/json");
			nlohmann::json j;
			j["jwt"] = Auth::generate_token(user.at("username"), user.at("password"));
			res.body = j.dump();
			res.status = 200;
		} else {
			res.status = 403;
		}
		});

	svr.Get((g_config.API_BASE_URL() + "/auth").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		//just a test to try to validate generated tokens
		set_response_headers(res);

		if (Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 200;
		} else {
			res.status = 403;
		}

		});
}
