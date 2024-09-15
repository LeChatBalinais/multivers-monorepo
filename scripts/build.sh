MODE="$1"
if  test "$MODE" = "release"
    then
        bazel build  //sandbox-describe:sandbox-describe
    else
        bazel build --compilation_mode=dbg --strip=never //sandbox-describe:sandbox-describe
fi