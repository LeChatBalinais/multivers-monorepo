""" """
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def immer():
    http_archive(
        name = "immer",
        strip_prefix = "immer-0.8.1",
        sha256 = "d1d5a1d3adcaaae9f34e01c155350c1488c720fb6e4dfa41ef88bf9ebd1e99ad",
        url = "https://github.com/arximboldi/immer/archive/refs/tags/v0.8.1.zip",
        build_file = "@//:third-party/immer/BUILD",
    )


immer_ext = module_extension(
    implementation = lambda ctx: immer(),
)