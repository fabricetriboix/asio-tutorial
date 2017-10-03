cmake_minimum_required(VERSION 3.1)
project(tutorial-asio-simple)

add_compile_options(-Wall -Wextra -Werror -std=c++14 -Wno-unused-parameter)

add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.60 REQUIRED COMPONENTS system log)

add_library(rtimer STATIC rtimer-impl.cpp rtimer-service.cpp rtimer-io.cpp)
target_link_libraries(rtimer Boost::log Boost::system rt)

add_executable(test-rtimer rtimer-test.cpp)
target_link_libraries(test-rtimer rtimer)
