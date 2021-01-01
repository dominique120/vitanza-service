cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(FS_LOCAL "Save files locally" OFF)
option(FS_S3 "Save files on S3" OFF)


project(vitanza-service CXX)

add_subdirectory(VitanzaService)
add_executable(vts ${vts_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(vts PROPERTIES CXX_STANDARD 17)
set_target_properties(vts PROPERTIES CXX_STANDARD_REQUIRED ON)

target_precompile_headers(vts PUBLIC VitanzaService/vtspch.h)

if(FS_LOCAL)
	target_compile_definitions(vts PUBLIC -DFS_LOCAL)
endif(FS_LOCAL)

if(FS_S3)
	target_compile_definitions(vts PUBLIC -DFS_S3)
endif(FS_S3)

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

find_package(AWSSDK REQUIRED COMPONENTS dynamodb s3)
find_package(OpenSSL 1.0.2 REQUIRED)
find_package(httplib REQUIRED)

target_include_directories(vts PRIVATE ${JWT_CPP_INCLUDE_DIRS} ${json_SOURCE_DIR})

target_link_libraries(vts PRIVATE        
		${AWSSDK_LINK_LIBRARIES}        
		nlohmann_json::nlohmann_json
		httplib::httplib
        )