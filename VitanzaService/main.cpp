#include "vtspch.h"
#include "main.h"

ConfigurationManager g_config;

int main() {
	std::cout << "Vitanza Service - Version 0.4a" << std::endl;
	std::cout << "Compiled with " << BOOST_COMPILER << std::endl;
	std::cout << "Compiled on " << __DATE__ << ' ' << __TIME__ << " for platform ";
	std::cout << BOOST_PLATFORM << "." << std::endl;


	std::cout << "Compiled for: "
#if defined(DB_DYNAMO)
		<< "DynamoDB." << std::endl;
	std::cout << "Aws sdk version: " << AWS_SDK_VERSION_STRING << std::endl;
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


	/*----------------- Authentication -------------------------------*/
	// later on add support for creating new password, deleting user, etc
	mux.handle(g_config [ "API_BASE_URL" ] + "/auth/users")
		.post([](served::response& res, const served::request& req) {
			// new user
			if (true) {
				res.set_status(201);
			} else {
				res.set_status(400);
			}
		});
	mux.handle(g_config [ "API_BASE_URL" ] + "/auth")
		.post([](served::response& res, const served::request& req) {
			const auto j = nlohmann::json::parse(req.body());
			std::map<std::string, std::string> user = j;			
			if (auth_wrapper::authenticate(user["username"], user["password"])) {
				res << Auth::generate_token(user [ "username" ], user [ "password" ]);
				res.set_status(200);
			} else {
				res.set_status(403);
			}
		});

	/*--------------- Customers ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/customers/{id}")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Client_wrapper::get_client(req.params [ "id" ]);
			if(response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.put([](served::response& res, const served::request& req) {
			if(Client_wrapper::update_client(req.params [ "id" ], req.body())) {
				res.set_status(204);
			} else {
				res.set_status(400);
			}	
		})
		.del([](served::response& res, const served::request& req) {
			if(Client_wrapper::delete_client(req.params [ "id" ])) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/customers")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Client_wrapper::get_all_clients();
			if (response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.post([](served::response& res, const served::request& req) {			
			if(Client_wrapper::new_client(req.body())) {
				res.set_status(201);
			} else {
				res.set_status(400);
			}
		});


	/*--------------- Products ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/products/{id}")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Product_wrapper::get_product(req.params [ "id" ]);
			if(response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.put([](served::response& res, const served::request& req) {
			if(Product_wrapper::update_product(req.params [ "id" ], req.body())) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}
		})
		.del([](served::response& res, const served::request& req) {
			if(Product_wrapper::delete_product(req.params [ "id" ])) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/products")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Product_wrapper::get_all_products();
			if(response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.post([](served::response& res, const served::request& req) {
			if(Product_wrapper::new_product(req.body())) {
				res.set_status(201);
			} else {
				res.set_status(400);
			}
		});

	/*--------------- Orders ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/orders/outstanding/")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Order_wrapper::get_outstanding_orders();
			if(response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		});
	
	mux.handle(g_config [ "API_BASE_URL" ] + "/orders/{id}")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = Order_wrapper::get_order(req.params [ "id" ]);
			if (response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.put([](served::response& res, const served::request& req) {
			if (Order_wrapper::update_order(req.params [ "id" ], req.body())) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}			
		})
		.del([](served::response& res, const served::request& req) {
			if(Order_wrapper::delete_order(req.params [ "id" ])) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}
		 });

	mux.handle(g_config [ "API_BASE_URL" ] + "/orders")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			const std::string response = Order_wrapper::get_all_orders();
			if (response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.post([](served::response& res, const served::request& req) {
			if(Order_wrapper::new_order(req.body())) {
				res.set_status(201);
			} else {
				res.set_status(400);
			}			
		 });

	/*--------------- Order Details ---------------------------*/
	mux.handle(g_config [ "API_BASE_URL" ] + "/orderdetails/{id}")
		.get([](served::response& res, const served::request& req) {
			res.set_header("Content-type", "application/json");
			res.set_header("Access-control-allow-origin", "*");
			const std::string response = OrderDetail_wrapper::get_order_detail(req.params [ "id" ]);
			if (response.empty()) {
				res.set_status(204);
			} else {
				res.set_status(200);
				res << response;
			}
		})
		.put([](served::response& res, const served::request& req) {
			if(OrderDetail_wrapper::update_order_detail(req.params [ "id" ], req.body())) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}			
		})
		.del([](served::response& res, const served::request& req) {
			if(OrderDetail_wrapper::delete_order_detail(req.params [ "id" ])) {
				res.set_status(200);
			} else {
				res.set_status(400);
			}
		});

	mux.handle(g_config [ "API_BASE_URL" ] + "/orderdetails")
		.get([](served::response& res, const served::request& req) {
		//implement a return all order details after providing a filter( order header id )
		})
		.post([](served::response& res, const served::request& req) {
			if(OrderDetail_wrapper::new_order_detail(req.body())) {
				res.set_status(201);
			} else {
				res.set_status(400);
			}
		});
}
