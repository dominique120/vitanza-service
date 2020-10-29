#include "vtspch.h"
#include "main.h"

ConfigurationManager g_config;

int main() {
	std::cout << "Vitanza Service - Version 0.3a" << std::endl;
	std::cout << "Compiled with " << BOOST_COMPILER << std::endl;
	std::cout << "Compiled on " << __DATE__ << ' ' << __TIME__ << " for platform ";
	std::cout << BOOST_PLATFORM << std::endl;

	std::cout << "Compiled for: "
#if defined(DB_DYNAMO)
		<< "DynamoDB." << std::endl;
#elif defined(DB_MYSQL)
		<< "MySQL." << std::endl;
#endif
		
	std::cout << "A microservice written by Dominique Verellen." << std::endl;
	std::cout << "Contact: dominique120@live.com." << std::endl;
	std::cout << std::endl;

	
	std::cout << "Initializing - Loading Configuration." << std::endl;
	g_config.load();
	

	std::cout << "Initializing - Setting up AWS SDK." << std::endl;
	const Aws::SDKOptions options;
	Aws::InitAPI(options);


	std::cout << "Initializing - Setting up multiplexer and registering handlers." << std::endl;
	served::multiplexer mux;
	register_handlers(mux);

	
	served::net::server server(g_config [ "SERVER_IP" ], g_config [ "SERVER_PORT" ], mux);
	std::cout << "Init done - Local address: " << g_config [ "SERVER_IP" ] << " bound using port " << g_config [ "SERVER_PORT" ] <<std::endl;
	server.run(10);
	
	Aws::ShutdownAPI(options);
	return (EXIT_SUCCESS);
}

void register_handlers(served::multiplexer& mux) {
	// register a more specialized route first, otherwise all requests with
	// "/customers" prefix will be routed to "/customers" handlers


	/*--------------- Customers ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/customers/{id}")
		.get([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			res.set_header("Content-type", "application/json");
			res << client_wrapper.get_client(req.params [ "id" ]);			
		})
		.put([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			bool success = client_wrapper.update_client(req.params [ "id" ], req.body());
			std::string s;
			s = success;
			res << s.c_str();
		})
		.del([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			bool success = client_wrapper.delete_client(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s.c_str();
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/customers")
		.get([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			res.set_header("Content-type", "application/json");
			res << client_wrapper.get_all_clients();
		})
		.post([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			bool success = client_wrapper.new_client(req.body());
			std::string s;
			s = success;
			res << s.c_str();
		});


	/*--------------- Products ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/products/{id}")
		.get([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			res.set_header("Content-type", "application/json");
			res << product_wrapper.get_product(req.params [ "id" ]);
		})
		.put([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			bool success = product_wrapper.update_product(req.params [ "id" ], req.body());
			std::string s;
			s = success;
			res << s.c_str();
		})
		.del([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			bool success = product_wrapper.delete_product(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s.c_str();
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/products")
		.get([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			res.set_header("Content-type", "application/json");
			res << product_wrapper.get_all_products();
		})
		.post([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			bool success = product_wrapper.new_product(req.body());
			std::string s;
			s = success;
			res << s.c_str();
		});

	/*--------------- Orders ---------------------------*/

	mux.handle(g_config [ "API_BASE_URL" ] + "/orders/outstanding/")
		.get([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			res.set_header("Content-type", "application/json");
			res << order_wrapper.get_outstanding_orders();
		});
	
	mux.handle(g_config [ "API_BASE_URL" ] + "/orders/{id}")
		.get([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			res.set_header("Content-type", "application/json");
			res << order_wrapper.get_order(req.params [ "id" ]);
		})
		.put([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			bool success = order_wrapper.update_order(req.params [ "id" ], req.body());
			std::string s;
			s = success;
			res << s.c_str();
		})
		.del([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			bool success = order_wrapper.delete_order(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s.c_str();
		 });

	mux.handle(g_config [ "API_BASE_URL" ] + "/orders")
		.get([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			res.set_header("Content-type", "application/json");
			res << order_wrapper.get_all_orders();
		})
		.post([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			bool success = order_wrapper.new_order(req.body());
			std::string s;
			s = success;
			res << s.c_str();
		 });

	/*--------------- Order Details ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/orderdetails/{id}")
		.get([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			res.set_header("Content-type", "application/json");
			res << order_detail_wrapper.get_order_detail(req.params [ "id" ]);
		})
		.put([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			bool success = order_detail_wrapper.update_order_detail(req.params [ "id" ], req.body());
			std::string s;
			s = success;
			res << s.c_str();
		})
		.del([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			bool success = order_detail_wrapper.delete_order_detail(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s.c_str();
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/orderdetails")
		.get([](served::response& res, const served::request& req) {
		//implement a return all order details after providing a filter( order header id )
		})
		.post([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			bool success = order_detail_wrapper.new_order_detail(req.body());
			std::string s;
			s = success;
			res << s.c_str();
		});
}
