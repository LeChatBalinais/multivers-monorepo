MODE="$1"
if  test "$MODE" = "release"
    then
        bazel build  //multivers-test:multivers-test
    else
        bazel build --compilation_mode=dbg --strip=never //multivers-test:multivers-test
fi