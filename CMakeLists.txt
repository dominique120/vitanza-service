cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(DB_DYNAMO "Use DynamoDB" ON)
option(DB_MYSQL "Use MySQL" OFF)

project(vitanza-service CXX)

add_subdirectory(VitanzaService)
add_executable(vts ${vts_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(vts PROPERTIES CXX_STANDARD 14)
set_target_properties(vts PROPERTIES CXX_STANDARD_REQUIRED ON)

if(DB_DYNAMO)
	target_compile_definitions(vts PUBLIC -DDB_DYNAMO)
endif(DB_DYNAMO)

if(DB_MYSQL)
	target_compile_definitions(vts PUBLIC -DDB_MYSQL)
endif(DB_MYSQL)

if(DB_MYSQL AND DB_DYNAMO)
	message(FATAL_ERROR "Specify only one database, you are using multiple options." )
endif(DB_MYSQL AND DB_DYNAMO)

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
    add_compile_options(-Wall -Werror -pipe -fvisibility=hidden)
endif ()

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fno-strict-aliasing)
endif ()

find_package(AWSSDK REQUIRED COMPONENTS dynamodb)
find_package(MySQL REQUIRED)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system )

include_directories(${Boost_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} ${json_SOURCE_DIR})

target_link_libraries(vts PRIVATE
        Boost::system
		${AWSSDK_LINK_LIBRARIES}
        ${MYSQL_CLIENT_LIBS}
		nlohmann_json::nlohmann_json
		served libserved.so
        )