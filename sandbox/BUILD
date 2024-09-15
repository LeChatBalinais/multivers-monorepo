cc_binary(
    name = "sandbox",
    srcs = glob(["**/*.hpp"])+ glob(["**/*.h"]) + glob(["**/*.cc"]),
    deps = [
        "@boost//:hana",
        "@boost//:intrusive",
        "@boost//:intrusive_ptr",
        "@boost//:optional",
        "@immer",
        "@lager",
        "@zug",
        "//multivers"
    ],
)
