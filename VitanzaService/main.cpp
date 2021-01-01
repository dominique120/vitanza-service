/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "main.h"

ConfigurationManager g_config;

int main(int argc, char* argv[]) {
	std::cout << "Vitanza Service - Version 0.5.2a" << std::endl;	
	std::cout << "Compiled on " << __DATE__ << ' ' << __TIME__ << "\n";


	std::cout << "Compiled for: " << "DynamoDB." << std::endl;
	std::cout << "Aws sdk version: " << AWS_SDK_VERSION_STRING << std::endl;

	std::cout << "A microservice written by Dominique Verellen." << std::endl;
	std::cout << "Contact: dominique120@live.com." << std::endl;
	std::cout << std::endl;


	std::cout << "Initializing - Loading Configuration." << std::endl;
	g_config.load(argc, argv);



#if defined(FS_S3)
	std::cout << "Using " << "S3 to store files remotely." << std::endl;
#elif defined(FS_LOCAL)
	if (std::filesystem::exists(g_config["FS_LOCAL_DIR"])) {
		std::cout << "Using " << "directory \"" << g_config["FS_LOCAL_DIR"] + "\" to store files locally." << std::endl;
	}
	else {
		if (std::filesystem::create_directory(g_config["FS_LOCAL_DIR"])) {
			std::cout << "Created directory \"" << g_config["FS_LOCAL_DIR"] + "\" to store files locally." << std::endl;
		}
		else {
			std::cout << "Could not create directory." << std::endl;
			return EXIT_FAILURE;
		}
	}
#else
	std::cout << "No file storage defined." << std::endl;
#endif


	std::cout << "Initializing - Setting up AWS SDK." << std::endl;
	const Aws::SDKOptions options;
	Aws::InitAPI(options);


	std::cout << "Initializing - Registering handlers." << std::endl;
	httplib::Server server;
	register_handlers(server);

	server.set_logger([](const auto& req, const auto& res) {
		Logger::log_event(req, res);
		});

	const auto ret = server.set_mount_point("/img_store", "./img_store");
	if (!ret) {
		std::cout << "Directory does not exist \n";
	}

	std::cout << "Init done - Local address: " << g_config["SERVER_IP"] << " bound using port " << g_config["SERVER_PORT"] << std::endl;
	server.listen(g_config["SERVER_IP"].c_str(), std::stoi(g_config["SERVER_PORT"]));


	Aws::ShutdownAPI(options);
	std::cout << "Exiting..." << std::endl;
	return (EXIT_SUCCESS);
}

void register_handlers(httplib::Server& svr) {
	svr.Options(R"(.*)", [&](const httplib::Request& req, httplib::Response& res) {
		res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE, PUT");
		res.set_header("Access-Control-Allow-Origin", "http://vts-alb-316342429.us-east-1.elb.amazonaws.com");
		res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
		});

	svr.Post((g_config["API_BASE_URL"] + "/config").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (g_config.reload(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	svr.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
		res.status = 200;
		});

	/*----------------- Authentication -------------------------------*/
	// later on add support for creating new password, deleting user, etc
	svr.Post((g_config["API_BASE_URL"] + "/auth/users").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}
		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);

		if (Auth::create_user(user.at("username"), user.at("password"))) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/auth/validateuser").c_str(), [](const httplib::Request& req, httplib::Response& res) {

		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);

		if (Auth::validate_user(user.at("username"), user.at("password"))) {
			res.status = 200;
		}
		else {
			res.status = 403;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/auth").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		const std::map<std::string, std::string> user = nlohmann::json::parse(req.body);
		if (auth_wrapper::authenticate(user.at("username"), user.at("password"))) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-Control-Allow-Origin", "*");
			nlohmann::json j;
			j["jwt"] = Auth::generate_token(user.at("username"), user.at("password"));
			res.body = j.dump();
			res.status = 200;
		}
		else {
			res.status = 403;
		}
		});

	svr.Get((g_config["API_BASE_URL"] + "/auth").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		//just a test to try to validate generated tokens
		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");

		if (Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 200;
		}
		else {
			res.status = 403;
		}

		});

	/*--------------- Customers ---------------------------*/
	svr.Get((g_config["API_BASE_URL"] + "/customers").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");

		if (req.has_param("id")) {
			const std::string response = Client_wrapper::get_client(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			const std::string response = Client_wrapper::get_all_clients();
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		});

	svr.Put((g_config["API_BASE_URL"] + "/customers").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (req.has_param("id")) {
			if (Client_wrapper::update_client(req.get_param_value("id"), req.body)) {
				res.status = 204;
			}
			else {
				res.status = 400;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Delete((g_config["API_BASE_URL"] + "/customers").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}
		if (req.has_param("id")) {
			if (Client_wrapper::delete_client(req.get_param_value("id"))) {
				res.status = 200;
			}
			else {
				res.status = 400;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/customers").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Client_wrapper::new_client(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	/*--------------- Products ---------------------------*/
	svr.Get((g_config["API_BASE_URL"] + "/products").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");

		if (req.has_param("id")) {
			const std::string response = Product_wrapper::get_product(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			const std::string response = Product_wrapper::get_all_products();
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		});

	svr.Put((g_config["API_BASE_URL"] + "/products").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Product_wrapper::update_product(req.get_param_value("id"), req.body)) {
			res.status = 200;
		}
		else {
			res.status = 400;
		}
		});

	svr.Delete((g_config["API_BASE_URL"] + "/products").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Product_wrapper::delete_product(req.get_param_value("id"))) {
			res.status = 200;
		}
		else {
			res.status = 400;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/products").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Product_wrapper::new_product(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	/*--------------- Orders ---------------------------*/
	svr.Get((g_config["API_BASE_URL"] + "/orders/by_client").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");

		if (req.has_param("id")) {
			const std::string response = Order_wrapper::get_order_by_client(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Get((g_config["API_BASE_URL"] + "/orders/outstanding").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");
		const std::string response = Order_wrapper::get_outstanding_orders();
		if (response.empty()) {
			res.status = 204;
		}
		else {
			res.status = 200;
			res.body = response;
		}
		});

	svr.Get((g_config["API_BASE_URL"] + "/orders").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");
		if (req.has_param("id")) {
			const std::string response = Order_wrapper::get_order(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			const std::string response = Order_wrapper::get_all_orders();
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		});

	svr.Put((g_config["API_BASE_URL"] + "/orders").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Order_wrapper::update_order(req.get_param_value("id"), req.body)) {
			res.status = 200;
		}
		else {
			res.status = 400;
		}
		});

	svr.Delete((g_config["API_BASE_URL"] + "/orders").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Order_wrapper::delete_order(req.get_param_value("id"))) {
			res.status = 200;
		}
		else {
			res.status = 400;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/orders").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (Order_wrapper::new_order(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	/*--------------- Order Details ---------------------------*/
	svr.Get((g_config["API_BASE_URL"] + "/orderdetails/by_order").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");
		if (req.has_param("id")) {
			const std::string response = OrderDetail_wrapper::get_orderdetails_by_order(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Get((g_config["API_BASE_URL"] + "/orderdetails").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		res.set_header("Content-type", "application/json");
		res.set_header("Access-Control-Allow-Origin", "*");
		if (req.has_param("id")) {
			const std::string response = OrderDetail_wrapper::get_order_detail(req.get_param_value("id"));
			if (response.empty()) {
				res.status = 204;
			}
			else {
				res.status = 200;
				res.body = response;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Put((g_config["API_BASE_URL"] + "/orderdetails").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}
		if (req.has_param("id")) {
			if (OrderDetail_wrapper::update_order_detail(req.get_param_value("id"), req.body)) {
				res.status = 200;
			}
			else {
				res.status = 400;
			}
		}
		else {
			res.status = 400;
		}
		});

	svr.Delete((g_config["API_BASE_URL"] + "/orderdetails").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (OrderDetail_wrapper::delete_order_detail(req.get_param_value("id"))) {
			res.status = 200;
		}
		else {
			res.status = 400;
		}
		});

	svr.Post((g_config["API_BASE_URL"] + "/orderdetails").c_str(), [](const httplib::Request& req, httplib::Response& res) {
		if (!Auth::validate_token(req.get_header_value("Authorization"))) {
			res.status = 403;
			return;
		}

		if (OrderDetail_wrapper::new_order_detail(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

}
