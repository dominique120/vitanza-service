set(vts_SRC
	${CMAKE_CURRENT_LIST_DIR}/Auth.cpp
	${CMAKE_CURRENT_LIST_DIR}/auth_wrapper.cpp
	${CMAKE_CURRENT_LIST_DIR}/config.cpp
	${CMAKE_CURRENT_LIST_DIR}/db_dynamo.cpp
	${CMAKE_CURRENT_LIST_DIR}/Entities.cpp
	${CMAKE_CURRENT_LIST_DIR}/file_wrapper.cpp
	${CMAKE_CURRENT_LIST_DIR}/local_file_handler.cpp
	${CMAKE_CURRENT_LIST_DIR}/logger.cpp	
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/request_handlers.cpp
	${CMAKE_CURRENT_LIST_DIR}/s3_file_handler.cpp
	PARENT_SCOPE)

