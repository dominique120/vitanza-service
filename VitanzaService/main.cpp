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
	if (std::filesystem::exists(g_config.FS_LOCAL_DIR())) {
		std::cout << "Using " << "directory \"" << g_config.FS_LOCAL_DIR() + "\" to store files locally." << std::endl;
	} else {
		if (std::filesystem::create_directory(g_config.FS_LOCAL_DIR())) {
			std::cout << "Created directory \"" << g_config.FS_LOCAL_DIR() + "\" to store files locally." << std::endl;
		} else {
			std::cout << "Could not create directory." << std::endl;
			return EXIT_FAILURE;
		}
	}
#else
	std::cout << "No file storage defined." << std::endl;
#endif


	std::cout << "Initializing - Setting up AWS SDK." << std::endl;
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	std::cout << "Initializing - Registering handlers." << std::endl;
	httplib::Server server;
	register_handlers_v2(server);

	server.set_logger([](const auto& req, const auto& res) {
		Logger::log_event(req, res);
		});

	std::cout << "Init done - Local address: " << g_config.SERVER_IP() << " bound using port " << g_config.SERVER_PORT() << std::endl;
	server.listen(g_config.SERVER_IP().c_str(), g_config.SERVER_PORT());


	Aws::ShutdownAPI(options);
	std::cout << "Exiting..." << std::endl;
	return (EXIT_SUCCESS);
}

void register_handlers_v2(httplib::Server& svr) {
	RequestHandlers::set_authentication_handlers(svr);
	RequestHandlers::set_client_handlers(svr);
	RequestHandlers::set_filter_change_handlers(svr);
	RequestHandlers::set_filter_installation_handlers(svr);
	RequestHandlers::set_note_handlers(svr);
	RequestHandlers::set_order_detail_handlers(svr);
	RequestHandlers::set_order_handlers(svr);
	RequestHandlers::set_product_handlers(svr);
}

/* TODO:
* - Add method for de activating a customer -> /customers/deactivate?id={id}
* - Add functionality to "create order" to subtract from stock
* - Add functionality to manage inventory(stock)
* - Add to database order table infor for ammount of items, total price and maybe extras
* - Add "archived" field to orders table in database
*/

