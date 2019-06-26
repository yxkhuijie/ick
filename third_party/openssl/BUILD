licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ssl",
    srcs = [
      "lib64/libssl.a",
    ],
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include/"],
)

cc_library(
    name = "crypto",
    srcs = [
      "lib64/libcrypto.a"
    ],
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include/"],
)

