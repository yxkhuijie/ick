licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libxerces",
    srcs = [
      "lib64/libxerces-c-3.2.a",
    ],
    hdrs = glob([
        "include/*.hpp",
        "include/**/*.hpp",
        "include/**/*.c",
        "include/**/*.cpp",
    ]),
    includes = ["include/"],
    linkopts = [
        "-ldl",
        "-lrt",
        "-lm",
    ],
)
