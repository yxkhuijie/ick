cmake_minimum_required(VERSION 3.6.0)

project(ICK)

option(ENABLE_GRPC  "option for use grpc" OFF)
option(ENABLE_MYSQL "option for enable mysql" OFF)
option(ENABLE_GFLAG "option for enable gflag" ON)
if (ENABLE_GRPC)
  add_definitions(-DENABLE_GRPC)
endif()
if (ENABLE_MYSQL)
  add_definitions(-DENABLE_DATABASE)
  add_definitions(-DENABLE_MYSQL)
endif()

include_directories("${ICK_SOURCE_DIR}")

set(ICK_CORE_SRC_FILE
  src/application.cc
  src/cmd_target.cc
  src/configure.cc
  src/control_object.cc
  src/converter.cc
  src/data_type.cc
  src/driver.cc
  src/file.cc
  src/int_value_info.cc
  src/logger.cc
  src/managed_object.cc
  src/message_entity.cc
  src/mutex.cc
  src/object.cc
  src/object_manager.cc
  src/read_only_double.cc
  src/read_only_int.cc
  src/read_only_string.cc
  src/read_write_double.cc
  src/read_write_int.cc
  src/read_write_string.cc
  src/service.cc
  src/service_info.cc
  src/service_param.cc
  src/subscriber.cc
  src/thread.cc
  src/timer.cc
  src/time_converter.cc
  src/type_info.cc
  src/untyped_data.cc
  src/value_info.cc
  src/xml_parser.cc
  src/xml_parse_handler.cc
)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(COMMON_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common/third_party")

if (ENABLE_GFLAG)
  add_definitions(-DENABLE_GFLAG)
  include_directories("${COMMON_THIRD_PARTY_DIR}/gflag/gflags-2.2.2/include")
  add_subdirectory("${COMMON_THIRD_PARTY_DIR}/gflag/gflags-2.2.2")
endif()

# add xercesc
include_directories("${COMMON_THIRD_PARTY_DIR}/xercesc/include")
add_subdirectory("${COMMON_THIRD_PARTY_DIR}/xercesc/xerces-c-3.2.2")
# add absl
add_subdirectory("${COMMON_THIRD_PARTY_DIR}/absl/abseil-cpp-master")
include_directories("${COMMON_THIRD_PARTY_DIR}/absl/abseil-cpp-master")

add_library(ick SHARED ${ICK_CORE_SRC_FILE})
