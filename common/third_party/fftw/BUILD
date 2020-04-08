licenses(["notice"])

cc_binary(
    name = "fft_test",
    srcs = [
        "src/fft_test.cc",
    ],
    deps = [
        ":fft",
    ]
)

cc_library(
    name = "fft",
    srcs = [
        "src/fft.cc",
    ],
    hdrs = [
        "src/fft.h",
    ],
    deps = [
        ":fftw",
    ],
    includes = [
        "src",
    ]
)

cc_library(
    name = "fftw",
    srcs = glob([
        "fftw-3.3.8/api/*.c",
        "fftw-3.3.8/dft/*.c",
        "fftw-3.3.8/dft/scalar/*.c",
        "fftw-3.3.8/dft/scalar/codelets/*.c",
        "fftw-3.3.8/dft/simd/*.c",
        "fftw-3.3.8/kernel/*.c",
        "fftw-3.3.8/rdft/*.c",
        "fftw-3.3.8/rdft/scalar/*.c",
        "fftw-3.3.8/rdft/scalar/r2cb/*.c",
        "fftw-3.3.8/rdft/scalar/r2cf/*.c",
        "fftw-3.3.8/rdft/scalar/r2r/*.c",
        "fftw-3.3.8/rdft/simd/*.c",
        "fftw-3.3.8/reodft/*.c",
        "fftw-3.3.8/simd-support/*.c",
        "fftw-3.3.8/libbench2/*.c",
        "fftw-3.3.8/reodft/*.c",
    ]) + select({
        ":use_sse2":glob([
            "fftw-3.3.8/dft/simd/sse2/*.c",
            "fftw-3.3.8/rdft/simd/sse2/*.c",
        ]),
        ":use_avx":glob([
            "fftw-3.3.8/dft/simd/avx/*.c",
            "fftw-3.3.8/rdft/simd/avx/*.c",
        ]),
        ":use_avx2":glob([
            "fftw-3.3.8/dft/simd/avx2/*.c",
            "fftw-3.3.8/dft/simd/avx2-128/*.c",
            "fftw-3.3.8/rdft/simd/avx2/*.c",
            "fftw-3.3.8/rdft/simd/avx2-128/*.c",
        ]),
        "//conditions:default": [],
    }),
    hdrs = glob([ 
        "fftw-3.3.8/api/*.h",
        "fftw-3.3.8/dft/*.h",
        "fftw-3.3.8/dft/scalar/*.h",
        "fftw-3.3.8/dft/scalar/codelets/*.h",
        "fftw-3.3.8/dft/simd/*.h",
        "fftw-3.3.8/kernel/*.h",
        "fftw-3.3.8/rdft/*.h",
        "fftw-3.3.8/rdft/scalar/*.h",
        "fftw-3.3.8/rdft/scalar/r2cb/*.h",
        "fftw-3.3.8/rdft/scalar/r2cf/*.h",
        "fftw-3.3.8/rdft/scalar/r2r/*.h",
        "fftw-3.3.8/rdft/simd/*.h",
        "fftw-3.3.8/reodft/*.h",
        "fftw-3.3.8/simd-support/*.h",
        "fftw-3.3.8/libbench2/*.h",
        "fftw-3.3.8/reodft/*.h",
        "include/*.h"
    ]) + select({
        ":use_sse2":glob([
            "fftw-3.3.8/dft/simd/sse2/*.h",
            "fftw-3.3.8/dft/simd/common/*.c",
            "fftw-3.3.8/rdft/simd/sse2/*.h",
            "fftw-3.3.8/rdft/simd/common/*.c",
        ]),
        ":use_avx":glob([
            "fftw-3.3.8/dft/simd/avx/*.h",
            "fftw-3.3.8/dft/simd/common/*.c",
            "fftw-3.3.8/rdft/simd/avx/*.h",
            "fftw-3.3.8/rdft/simd/common/*.c",
        ]),
        ":use_avx2":glob([
            "fftw-3.3.8/dft/simd/avx2/*.h",
            "fftw-3.3.8/dft/simd/avx2-128/*.h",
            "fftw-3.3.8/dft/simd/common/*.c",
            "fftw-3.3.8/rdft/simd/avx2/*.h",
            "fftw-3.3.8/rdft/simd/avx2-128/*.h",
            "fftw-3.3.8/rdft/simd/common/*.c",
        ]),
        "//conditions:default": [],
    }),
    includes = [
        "fftw-3.3.8",
        "include",
    ],
    copts = [] + select({
        ":use_sse2":["-msse2 -DHAVE_SSE2"],
        ":use_avx":["-mavx -DHAVE_AVX"],
        ":use_avx2":["-mavx2 -mfma -DHAVE_AVX2 -DHAVE_FMA"],
        "//conditions:default": [],
    })
)

config_setting(
    name = "use_sse2",
    values = {"define":"ENABLE_SSE2=ON"}
)

config_setting(
    name = "use_avx",
    values = {"define":"ENABLE_AVX=ON"}
)

config_setting(
    name = "use_avx2",
    values = {"define":"ENABLE_AVX2=ON"}
)

