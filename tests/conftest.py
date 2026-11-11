# Copyright 2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.


def pytest_addoption(parser):
    parser.addoption(
        "--xassert-build-xc-tests",
        choices=("on", "off"),
        default="on",
        help="Match the XASSERT_BUILD_XC_TESTS CMake option (off for VX)",
    )


def pytest_generate_tests(metafunc):
    if "control_project" in metafunc.fixturenames:
        projects = ["assertion_control_c_test"]
        if metafunc.config.getoption("--xassert-build-xc-tests") == "on":
            projects.append("assertion_control_xc_test")
        metafunc.parametrize("control_project", projects)
