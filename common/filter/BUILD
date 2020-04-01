licenses(["notice"])

cc_binary(
    name = "filter_test",
    srcs = [
        "filter_test.cc",
    ],
    deps = [
        ":butterworth_lowpass_filter",
    ]
)

cc_library(
    name = "butterworth_lowpass_filter",
    srcs = [
        "butterworth_lowpass_filter.cc",
    ],
    hdrs = [
        "butterworth_lowpass_filter.h",
    ],
    deps = [
        ":filter",
    ]
)

cc_library(
    name = "filter",
    srcs = [
        "filter.cc",
    ],
    hdrs = [
        "filter.h",
    ]
)

