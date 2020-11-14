cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(DB_DYNAMO "Use DynamoDB" OFF)
option(DB_MYSQL "Use MySQL" OFF)

project(vitanza-service CXX)

add_subdirectory(VitanzaService)
add_executable(vts ${vts_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(vts PROPERTIES CXX_STANDARD 14)
set_target_properties(vts PROPERTIES CXX_STANDARD_REQUIRED ON)

if (${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(vts PUBLIC VitanzaService/vtspch.h)
else ()
    include(cotire)
    set_target_properties(vts PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "VitanzaService/vtspch.h")
    set_target_properties(vts PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
    cotire(vts)
endif ()

if(DB_DYNAMO)
	target_compile_definitions(vts PUBLIC -DDB_DYNAMO)
endif(DB_DYNAMO)

if(DB_MYSQL)
	target_compile_definitions(vts PUBLIC -DDB_MYSQL)
endif(DB_MYSQL)

if(DB_MYSQL AND DB_DYNAMO)
	message(FATAL_ERROR "Specify only one database, you are using multiple options." )
endif(DB_MYSQL AND DB_DYNAMO)

if(!DB_MYSQL AND !DB_DYNAMO)
	message(FATAL_ERROR "Specify a database, you are not settign any. Use cmake -DDB_DYNAMO=ON or cmake -DDB_MYSQL=ON" )
endif(!DB_MYSQL AND !DB_DYNAMO)

find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

include(FetchContent)
LINK_DIRECTORIES(/usr/local/lib)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_TAG v3.9.1)
  
  FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if (NOT WIN32)
    add_compile_options(-Wall -Werror -pipe -fvisibility=hidden -Weverything)
endif ()

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

find_package(AWSSDK REQUIRED COMPONENTS dynamodb s3 sqs)
find_package(MySQL REQUIRED)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system)
find_package(OpenSSL 1.0.2 REQUIRED)
find_package(httplib REQUIRED)

target_include_directories(vts PRIVATE ${JWT_CPP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} ${json_SOURCE_DIR})

target_link_libraries(vts PRIVATE
        Boost::system
		${AWSSDK_LINK_LIBRARIES}
        ${MYSQL_CLIENT_LIBS}
		nlohmann_json::nlohmann_json
		httplib::httplib
        )