licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mysqlcppconn",
    srcs = [
      "lib64_release/libmysqlcppconn-static.a"
    ],
    hdrs = glob([
        "include/*.h",
        "include/**/*.h",
    ]),
    includes = ["include/"],
    linkopts = [
        "-ldl",
        "-lrt",
        "-lm",
    ],
)
