""" """
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def zug():
    http_archive(
       name = "zug",
        strip_prefix = "zug-0.1.1",
        sha256 = "0ebb21bc3dfbc79ce5027c33db5402aa188a9f5d5c66a4f8dd3c26722d5ad43e",
        url = "https://github.com/arximboldi/zug/archive/refs/tags/v0.1.1.zip",
        build_file = "@//:third-party/zug/BUILD",
    )


zug_ext = module_extension(
    implementation = lambda ctx: zug(),
)