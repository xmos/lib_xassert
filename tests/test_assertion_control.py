# Copyright 2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

from pathlib import Path

import Pyxsim
import pytest
from Pyxsim import testers


PROFILES = [
    ("GLOBAL_00", False, False),
    ("GLOBAL_01", False, True),
    ("GLOBAL_10", True, False),
    ("GLOBAL_11", True, True),
    ("DISABLE_REGULAR", False, True),
    ("DISABLE_TIMING", True, False),
    ("DISABLE_BOTH", False, False),
    ("ENABLE_REGULAR", True, False),
    ("ENABLE_TIMING", False, True),
    ("ENABLE_BOTH", True, True),
]


@pytest.mark.parametrize("profile, regular_enabled, timing_enabled", PROFILES)
@pytest.mark.parametrize("probe", ["REGULAR", "TIMING", "LOOP", "STATE"])
def test_assertion_control(
    control_project, profile, regular_enabled, timing_enabled, probe, capfd,
):
    config = f"{profile}_{probe}"
    binary = (
        Path(__file__).parent / control_project / "bin" / config
        / f"{control_project}_{config}.xe"
    )
    assert binary.is_file(), f"Build the assertion control tests first: missing {binary}"

    probe_traps = (
        (probe == "REGULAR" and regular_enabled)
        or (probe in ("TIMING", "LOOP") and timing_enabled)
    )
    expected = ["STATE OK"]
    if not probe_traps:
        expected.append("CONTINUED")
    expected.append("Unhandled exception: ECALL, data: 0x00000000")
    tester = testers.ComparisonTester(expected, regexp=False)
    result = Pyxsim.run_on_simulator_(
        binary,
        tester=tester,
        capfd=capfd,
        simargs=["--max-cycles", "15000000"],
        timeout=30,
    )
    assert result, "".join(tester.failures)
