#include "main.h"


int main() {
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	served::multiplexer mux;

	// register a more specialized route first, otherwise all requests with
	// "/customers" prefix will be routed to "/customers" handlers


	/*--------------- Customers ---------------------------*/
	mux.handle("/customers/{id}")
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
			res << s;
		})
		.del([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			bool success = client_wrapper.delete_client(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s;
		});

	mux.handle("/customers")
		.get([](served::response& res, const served::request& req) {
			//implement a return all values in table in dynamo
		})
		.post([](served::response& res, const served::request& req) {
			Client_wrapper client_wrapper;
			bool success = client_wrapper.new_client(req.body());
			std::string s;
			s = success;
			res << s;
		});


	/*--------------- Products ---------------------------*/
	mux.handle("/products/{id}")
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
			res << s;
		})
		.del([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			bool success = product_wrapper.delete_product(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s;
		});

	mux.handle("/products")
		.get([](served::response& res, const served::request& req) {
			//implement a return all values in table in dynamo
		})
		.post([](served::response& res, const served::request& req) {
			Product_wrapper product_wrapper;
			bool success = product_wrapper.new_product(req.body());
			std::string s;
			s = success;
			res << s;
		});

	/*--------------- Orders ---------------------------*/
	mux.handle("/orders/{id}")
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
			res << s;
		})
		.del([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			bool success = order_wrapper.delete_order(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s;
		 });

	mux.handle("/orders")
		.get([](served::response& res, const served::request& req) {
			//implement a return all values in table in dynamo
		})
		.post([](served::response& res, const served::request& req) {
			Order_wrapper order_wrapper;
			bool success = order_wrapper.new_order(req.body());
			std::string s;
			s = success;
			res << s;
		 });

		/*--------------- Order Details ---------------------------*/
	mux.handle("/orders/{id}")
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
			res << s;
		})
		.del([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			bool success = order_detail_wrapper.delete_order_detail(req.params [ "id" ]);
			std::string s;
			s = success;
			res << s;
		});

	mux.handle("/orders")
		.get([](served::response& res, const served::request& req) {
		//implement a return all values in table in dynamo
		})
		.post([](served::response& res, const served::request& req) {
			OrderDetail_wrapper order_detail_wrapper;
			bool success = order_detail_wrapper.new_order_detail(req.body());
			std::string s;
			s = success;
			res << s;
		});


	served::net::server server("192.168.1.72", "8123", mux);
	server.run(10);
	
	Aws::ShutdownAPI(options);
	return (EXIT_SUCCESS);

}
