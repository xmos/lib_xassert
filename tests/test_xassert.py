# Copyright 2015-2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
from pathlib import Path
import subprocess
import pytest
import re

test_path = Path(__file__).parent


def normalize_main_xc_paths(text):
    # Normalize any absolute/relative path prefix so output can be compared
    # consistently across build locations and host platforms.
    return re.sub(r"\((?:.*[/\\\\])?main\.xc:(\d+)\)", r"(main.xc:\1)", text)


def assert_expected_output(dut, expected_output):
    # xsim output may contain host-specific main.xc paths; normalize first.
    dut_norm = normalize_main_xc_paths(dut).rstrip("\n")
    exp_norm = normalize_main_xc_paths(expected_output).rstrip("\n")

    exp_lines = exp_norm.splitlines()
    # If any expected line starts with "re:", compare line-by-line where
    # regex lines are matched with fullmatch and other lines are exact.
    # This is useful for timing assertions where numeric values can vary.
    if any(line.startswith("re:") for line in exp_lines):
        dut_lines = dut_norm.splitlines()
        assert len(dut_lines) == len(exp_lines)
        for i, (dut_line, exp_line) in enumerate(zip(dut_lines, exp_lines), start=1):
            if exp_line.startswith("re:"):
                assert re.fullmatch(exp_line[3:], dut_line), f"line {i} did not match regex: {exp_line[3:]}\nactual: {dut_line}"
            else:
                assert dut_line == exp_line
    else:
        assert dut_norm == exp_norm

@pytest.mark.parametrize("test_name", [b.stem for b in test_path.rglob("*.xe")])
def test_xassert(test_name):
    test_bin = [b for b in test_path.rglob(f"{test_name}.xe")][0]
    expect_file = test_path / f"{test_name}.expect"
    with open(expect_file, "r") as fp:
        expected_output = fp.read()
        cmd = ["xsim", test_bin]
        ret = subprocess.run(cmd, capture_output=True, text=True)

        assert_expected_output(ret.stdout, expected_output)
