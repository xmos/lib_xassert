set(LIB_NAME lib_xassert)
set(LIB_VERSION 5.0.0)
set(LIB_INCLUDES api)
set(LIB_DEPENDENT_MODULES "lib_basix(0.1.0)")
set(LIB_OPTIONAL_HEADERS debug_conf.h xassert_conf.h)

set(LIB_COMPILER_FLAGS  -Wall
                        -Wextra
                        -Wunused
                        # -Wpadded
                        -Wshadow
                        -Wconversion
                        -Wdiv-by-zero
                        -Wfloat-equal
                        -Wsign-compare
)


XMOS_REGISTER_MODULE()
