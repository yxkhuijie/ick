package(default_visibility = ['//visibility:public'])

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

