cc_library(
    name = "multivers",
    srcs = glob(["**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    strip_include_prefix = "include",
    includes = ["include/"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:optional",
        "@boost//:describe"
    ],
)
