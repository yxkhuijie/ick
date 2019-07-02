licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libcurl",
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include/"],
    deps = [
         "//third_party/curl/curl-7.60.0:curl",
    ],
)
