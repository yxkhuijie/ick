licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libevent",
    srcs = [
      "lib64/libevent.a",
      "lib64/libevent_core.a",
      "lib64/libevent_extra.a",
      "lib64/libevent_openssl.a",
      "lib64/libevent_pthreads.a",
    ],
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include/"],
)
