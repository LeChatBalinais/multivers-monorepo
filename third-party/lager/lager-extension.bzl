""" """
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def lager():
    http_archive(
         name = "lager",
        strip_prefix = "lager-0.1.1",
        sha256 = "976a4c0b09d03099ad333358a76ac2b2078025f7312dcb0bb39ae62ac3709506",
        url = "https://github.com/arximboldi/lager/archive/refs/tags/v0.1.1.zip",
        build_file = "@//:third-party/lager/BUILD",
    )


lager_ext = module_extension(
    implementation = lambda ctx: lager(),
)