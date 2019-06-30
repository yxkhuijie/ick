licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libcurl",
    srcs = [
      "lib64/libcurl.a",
    ],
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include/"],
    deps = [
        "//third_party/openssl:ssl",
        "//third_party/openssl:crypto",
        "//third_party/zlib:zlib",
    ],
    linkopts = [
        "-ldl",
        "-lrt",
        "-lm",
        "-lpthread",
        #"-lssl",
        #"-lcrypto",
    ],
)
