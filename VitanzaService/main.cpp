#include "main.h"
#include "client_wrapper.h"



int main() {
	Aws::SDKOptions options;
	Aws::InitAPI(options);
	served::multiplexer mux;

	/* ---------UUID Generation -----------------*/
	/*
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	std::cout << uuid << std::endl;
	std::string id = boost::uuids::to_string(uuid);
	*/



	// register a more specialized route first, otherwise all requests with
	// "/customers" prefix will be routed to "/customers" handlers



	/*--------------- Customers ---------------------------*/
	mux.handle("/customers/{id}")
		.get([](served::response& res, const served::request& req) {
		// Allocation
		nlohmann::json j;
		Client client;

		int id = std::stoi(req.params [ "id" ]);

		// Get the {id} variable from the path and 
		client = client.select_client_by_id(id);

		// Set j to the serialized json of what was returned in the query
		std::cout << "to_json" << std::endl;
		client.to_json(j, client);

		// Set the header for the response and dump the json to the response body.
		std::cout << "dump json to body" << std::endl;
		res.set_header("Content-type", "application/json");
		res << j.dump();
			 })
		.put([](served::response& res, const served::request& req) {
				 Client client;

				 int id = std::stoi(req.params [ "id" ]);

				 auto j = nlohmann::json::parse(req.body());
				 Client cl;
				 try {
					 cl.from_json(j, cl);
				 } catch (nlohmann::json::exception& e) {
					 std::cout << e.what();
				 }

				 std::cout << req.body();
				 cl.update_client(cl);

			 })
				 .del([](served::response& res, const served::request& req) {
				 Client cl;
				 int id = std::stoi(req.params [ "id" ]);
				 cl.delete_client(id);
					  });

			 mux.handle("/customers")
				 .get([](served::response& res, const served::request& req) {
				 Client client;
				 res.set_header("Content-type", "application/json");
				 res << client.to_json_array(client.select_clients());

					  })
				 .post([](served::response& res, const served::request& req) {

						  auto j = nlohmann::json::parse(req.body());
						  Client cl;
						  try {
							  cl.from_json(j, cl);
						  } catch (nlohmann::json::exception& e) {
							  std::cout << e.what();
						  }

						  std::cout << req.body();
						  bool success = cl.new_client(cl);
						  std::cout << "client creation was " << success << std::endl;
					   });


					  /*--------------- Products ---------------------------*/
					  mux.handle("/products/{id}")
						  .get([](served::response& res, const served::request& req) {
						  // Allocation
						  nlohmann::json j;
						  Product product;

						  int id = std::stoi(req.params [ "id" ]);

						  // Get the {id} variable from the path and 
						  product = product.select_product_by_id(id);

						  // Set j to the serialized json of what was returned in the query
						  product.to_json(j, product);

						  // Set the header for the response and dump the json to the response body.
						  res.set_header("Content-type", "application/json");
						  res << j.dump();

							   })
						  .put([](served::response& res, const served::request& req) {
								   Product product;

								   int id = std::stoi(req.params [ "id" ]);

								   auto j = nlohmann::json::parse(req.body());

								   try {
									   product.from_json(j, product);
								   } catch (nlohmann::json::exception& e) {
									   std::cout << e.what();
								   }

								   std::cout << req.body();
								   bool success = product.update_product(product);

							   })
								   .del([](served::response& res, const served::request& req) {
								   Product product;
								   int id = std::stoi(req.params [ "id" ]);
								   bool success = product.delete_product(id);
										});

							   mux.handle("/products")
								   .get([](served::response& res, const served::request& req) {
								   Product product;
								   res.set_header("Content-type", "application/json");
								   res << product.to_json_array(product.select_products());

										})
								   .post([](served::response& res, const served::request& req) {

											auto j = nlohmann::json::parse(req.body());
											Product product;
											try {
												product.from_json(j, product);
											} catch (nlohmann::json::exception& e) {
												std::cout << e.what();
											}
											std::cout << product.price << std::endl;
											std::cout << product.available_stock << std::endl;

											std::cout << req.body();
											bool success = product.new_product(product);
										 });

										/*--------------- Orders and Order Details ---------------------------*/
										mux.handle("/orders/{id}")
											.get([](served::response& res, const served::request& req) {
											// Allocation
											nlohmann::json j;
											Product product;

											int id = std::stoi(req.params [ "id" ]);

											// Get the {id} variable from the path and 
											product = product.select_product_by_id(id);

											// Set j to the serialized json of what was returned in the query
											product.to_json(j, product);

											// Set the header for the response and dump the json to the response body.
											res.set_header("Content-type", "application/json");
											res << j.dump();

												 })
											.put([](served::response& res, const served::request& req) {
													 Product product;

													 int id = std::stoi(req.params [ "id" ]);

													 auto j = nlohmann::json::parse(req.body());

													 try {
														 product.from_json(j, product);
													 } catch (nlohmann::json::exception& e) {
														 std::cout << e.what();
													 }

													 std::cout << req.body();
													 bool success = product.update_product(product);

												 })
													 .del([](served::response& res, const served::request& req) {
													 Product product;
													 int id = std::stoi(req.params [ "id" ]);
													 bool success = product.delete_product(id);
														  });

												 mux.handle("/orders")
													 .get([](served::response& res, const served::request& req) {
													 Product product;
													 res.set_header("Content-type", "application/json");
													 res << product.to_json_array(product.select_products()); //place serialized json here

														  })
													 .post([](served::response& res, const served::request& req) {

															  auto j = nlohmann::json::parse(req.body());
															  Product product;
															  try {
																  product.from_json(j, product);
															  } catch (nlohmann::json::exception& e) {
																  std::cout << e.what();
															  }
															  std::cout << product.price << std::endl;
															  std::cout << product.available_stock << std::endl;

															  std::cout << req.body();
															  bool success = product.new_product(product);
														   });




														  served::net::server server("192.168.1.72", "8123", mux);
														  server.run(10);

														  Aws::ShutdownAPI(options);
														  return (EXIT_SUCCESS);



}
