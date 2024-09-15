
MODE="$1"
if  test "$MODE" = "release"
    then
        bazel build  //sandbox:sandbox
    else
        bazel build --compilation_mode=dbg --strip=never //sandbox:sandbox
fi
