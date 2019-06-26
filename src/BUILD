package(default_visibility = ['//visibility:public'])

cc_binary(
  name = "ick_main_test",
  srcs = [
    "ick_main_test.cc",
  ],
  deps = [
    ":application",
  ]
)

cc_library(
  name = "application",
  srcs = [
    "application.cc",
    "connection_pool.cc",
    "connection_pool_manager.cc",
    "ick_storm_subscriber_socket.cc",
    "ick_storm_subscriber_manager.cc",
    "ick_storm_subscriber.cc",
    "ick_storm_service_socket.cc",
    "ick_storm_debugger_socket.cc",
    "ick_service_impl.cc",
    "ick_storm_publisher.cc",
    "ick_storm_manager.cc",
    "configure.cc",
    "data_base.cc",
    "socket.cc",
  ],
  hdrs = [
    "application.h",
    "connection_pool.h",
    "connection_pool_manager.h",
    "ick_storm_subscriber_socket.h",
    "ick_storm_subscriber_manager.h",
    "ick_storm_subscriber.h",
    "ick_storm_service_socket.h",
    "ick_storm_debugger_socket.h",
    "ick_service_impl.h",
    "ick_storm_publisher.h",
    "ick_storm_manager.h",
    "configure.h",
    "data_base.h",
    "socket.h",
  ],
  deps = [
    ":object",
    ":thread",
    ":logger",
    ":mutex",
    ":read_write_int",
    ":read_write_double",
    ":read_write_string",
    ":read_only_int",
    ":read_only_double",
    ":read_only_string",
    ":object_manager",
    ":xml_parser",
    ":mysql_connection",
    "@com_github_gflags_gflags//:gflags",
  ],
)

cc_library(
  name = "object",
  srcs = [
    "object.cc",
  ],
  hdrs = [
    "object.h",
  ]
)

cc_library(
  name = "logger",
  srcs = [
    "logger.cc",
  ],
  hdrs = [
    "logger.h",
  ],
  deps = [
    ":converter",
    ":object",
    ":thread",
    ":mutex",
    ":time_converter",
  ]
)

cc_library(
  name = "mutex",
  srcs = [
    "mutex.cc",
  ],
  hdrs = [
    "mutex.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "time_converter",
  srcs = [
    "time_converter.cc",
  ],
  hdrs = [
    "time_converter.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "converter",
  srcs = [
    "converter.cc",
  ],
  hdrs = [
    "converter.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "thread",
  srcs = [
    "thread.cc",
  ],
  hdrs = [
    "thread.h",
  ],
  deps = [
    ":object",
  ]
)

cc_test(
  name = "object_test",
  srcs = [
    "object_test.cc",
  ],
  deps = [
    "@googletest//:gtest_main",
    ":mysql_connection",
  ]
)

cc_library(
  name = "http_client",
  srcs = [
    "http_client.cc",
  ],
  hdrs = [
    "http_client.h",
  ],
  deps = [
    ":logger",
    "//third_party/curl:libcurl",
  ]
)

cc_test(
  name = "http_client_test",
  srcs = [
    "http_client_test.cc",
  ],
  deps = [
    ":http_client",
    "@googletest//:gtest_main",
  ]
)

cc_library(
  name = "mysql_connection",
  srcs = [
    "mysql_connection.cc",
  ],
  hdrs = [
    "mysql_connection.h",
  ],
  deps = [
    ":connection",
    ":logger",
    #"@boringssl//:ssl",
    #"@boringssl//:crypto",
    #"@zlib_archive//:zlib",
  ],
  linkopts = [
    "-ldl",
  ]
)

cc_library(
  name = "connection",
  srcs = [
    "connection.cc",
  ],
  hdrs = [
    "connection.h",
  ],
  deps = [
    ":object",
    "//third_party/mysql_client_cpp:mysqlcppconn",
  ]
)

cc_library(
  name = "control_object",
  srcs = [
    "control_object.cc",
    "service.cc",
    "service_param.cc",
    "service_info.cc",
  ],
  hdrs = [
    "control_object.h",
    "service.h",
    "service_param.h",
    "service_info.h",
  ],
  deps = [
    "read_write_int",
    "read_write_double",
    "read_write_string",
    "read_only_int",
    "read_only_double",
    "read_only_string",
  ]
)

cc_library(
  name = "object_manager",
  srcs = [
    "object_manager.cc",
    "driver.cc",
    "protocol.cc",
  ],
  hdrs = [
    "object_manager.h",
    "driver.h",
    "protocol.h",
  ],
  deps = [
    ":object",
    "control_object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_write_int",
  srcs = [
    "read_write_int.cc",
  ],
  hdrs = [
    "read_write_int.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_write_double",
  srcs = [
    "read_write_double.cc",
  ],
  hdrs = [
    "read_write_double.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_write_string",
  srcs = [
    "read_write_string.cc",
  ],
  hdrs = [
    "read_write_string.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_only_int",
  srcs = [
    "read_only_int.cc",
  ],
  hdrs = [
    "read_only_int.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_only_double",
  srcs = [
    "read_only_double.cc",
  ],
  hdrs = [
    "read_only_double.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "read_only_string",
  srcs = [
    "read_only_string.cc",
  ],
  hdrs = [
    "read_only_string.h",
  ],
  deps = [
    ":object",
    "untyped_data",
  ]
)

cc_library(
  name = "untyped_data",
  srcs = [
    "untyped_data.cc",
    "data_type.cc",
    "cmd_target.cc",
    "ick_storm_data.cc",
    "managed_object.cc",
    "value_info.cc",
    "int_value_info.cc",
    "type_info.cc",
    "subscriber.cc",
  ],
  hdrs = [
    "access_mode.h",
    "data_type.h",
    "untyped_data.h",
    "cmd_target.h",
    "ick_storm_data.h",
    "managed_object.h",
    "value_info.h",
    "int_value_info.h",
    "type_info.h",
    "subscriber.h",
  ],
  deps = [
    ":object",
    ":logger",
    ":mutex",
  ]
)

cc_library(
  name = "xml_parser",
  srcs = [
    "xml_parser.cc",
    "xml_parse_handler.cc",
  ],
  hdrs = [
    "xml_parser.h",
    "xml_parse_handler.h",
  ],
  deps = [
    ":object",
    ":read_write_int",
    ":read_write_double",
    ":read_write_string",
    ":read_only_int",
    ":read_only_double",
    ":read_only_string",
    ":object_manager",
    "//third_party/xercesc:libxerces",
  ]
)

