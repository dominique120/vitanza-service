cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(vitanza-service CXX)

add_subdirectory(VitanzaService)
add_executable(vts ${vts_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(vts PROPERTIES CXX_STANDARD 14)
set_target_properties(vts PROPERTIES CXX_STANDARD_REQUIRED ON)


if (NOT WIN32)
    add_compile_options(-Wall -Werror -pipe -fvisibility=hidden)
endif ()

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fno-strict-aliasing)
endif ()

find_package(AWSSDK REQUIRED COMPONENTS dynamodb)
find_package(MySQL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost 1.53.0 REQUIRED COMPONENTS asio system )
find_package(Served REQUIRED)



include_directories(${Boost_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} ${SERVED_INCLUDE_DIR})

target_link_libraries(tfs PRIVATE
        Boost::asio
        Boost::system
		${SERVED_LINK_LIBRARIES}
		${AWSSDK_LINK_LIBRARIES}
        ${MYSQL_CLIENT_LIBS}
		nlohmann_json::nlohmann_json
        )

