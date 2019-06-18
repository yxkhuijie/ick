package(default_visibility = ['//visibility:public'])

cc_library(
  name = "object",
  srcs = [
    "IObject.cpp",
  ],
  hdrs = [
    "IObject.h",
  ]
)

cc_library(
  name = "logger",
  srcs = [
    "Logger.cpp",
  ],
  hdrs = [
    "Logger.h",
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
    "IMutex.cpp",
  ],
  hdrs = [
    "IMutex.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "time_converter",
  srcs = [
    "TimeConverter.cpp",
  ],
  hdrs = [
    "TimeConverter.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "converter",
  srcs = [
    "Converter.cpp",
  ],
  hdrs = [
    "Converter.h",
  ],
  deps = [
    ":object",
  ]
)

cc_library(
  name = "thread",
  srcs = [
    "IThread.cpp",
  ],
  hdrs = [
    "IThread.h",
  ],
  deps = [
    ":object",
  ]
)

cc_test(
  name = "object_test",
  srcs = [
    "ObjectTest.cpp",
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
    "Connection.cpp",
  ],
  hdrs = [
    "Connection.h",
  ],
  deps = [
    ":object",
    "//third_party/mysql_client_cpp:mysqlcppconn",
  ]
)

