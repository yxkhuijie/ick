licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ssl",
    srcs = ["lib64_release/libssl.a"],
    hdrs = glob([
        "*.h",
        "**/*.h",
    ]),
    includes = ["."],
)

cc_library(
    name = "crypto",
    srcs = ["lib64_release/libcrypto.a"],
    hdrs = glob([
        "*.h",
        "**/*.h",
    ]),
    includes = ["."],
    linkopts = ["-ldl"],
)
