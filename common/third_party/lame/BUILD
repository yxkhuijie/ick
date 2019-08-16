licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "lame",
  srcs = glob([
    "lib64/*.a",
  ]),
  hdrs = glob([
    "include/*.h",
  ]),
  copts = [
    "--std=c++11",
  ],
  includes = [
    "include",
  ],
)
