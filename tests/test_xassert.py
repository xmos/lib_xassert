# Copyright 2015-2024 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
from pathlib import Path
import subprocess
import pytest
import re

test_path = Path(__file__).parent

@pytest.mark.parametrize("test_name", [b.stem for b in test_path.rglob("*.xe")])
def test_xassert(test_name):
   test_bin = [b for b in test_path.rglob(f"{test_name}.xe")][0]
   expect_file = test_path / f"{test_name}.expect"
   with open(expect_file, "r") as fp:
      expected_output = fp.read()
      cmd = ["xsim", test_bin]
      ret = subprocess.run(cmd, capture_output=True, text=True)

      if "main.xc" not in  ret.stdout:
         assert ret.stdout == expected_output
      else:
         '''
         For test cases where the filename and the line number of the assertion is printed in the stdout, comparison is trickier.
         When running a .xe compiled using xcommon_cmake, the stdout has the absolute path of the file where the assert happens. For example,
         x > 5 && xassert_msg("x is not greater than 5") (/Users/shuchitak/sandboxes/usb_audio_sandbox/lib_xassert/tests/assert_test/src/main.xc:8)
         While the reference .expect file has the relative file path.
         x > 5 && xassert_msg("x is not greater than 5") (../src/main.xc:8)
         So, to compare these 2, first we remove the (<path to main.xc>/main.xc:8) bit from the output and compare the rest for equality.
         Then from the (<path to main.xc>/main.xc:<line_no>), we extract only the main.xc:<line_no> substr and compare those for equality.

         '''
         dut = ret.stdout
         ref = expected_output
         pattern = r"\s+\(.*\/main\.xc:\d+\)"
         m = re.search(pattern, dut)
         if m:
            dut_main_xc = m.group() # Extract (<path to main.xc>/main.xc:<line_no>)
            n = re.search(r"main\.xc:\d+", dut_main_xc) # From that, extract just main.xc:<line_no>
            assert n
            dut_main_line_num = n.group()
            dut_without_main_xc = re.sub(pattern, '', dut) # Remove (<path to main.xc>/main.xc:<line_no>) from the string so the rest can be compared
         else:
            assert False

         m = re.search(pattern, ref)
         if m:
            ref_main_xc = m.group() # Extract (<path to main.xc>/main.xc:<line_no>)
            n = re.search(r"main\.xc:\d+", ref_main_xc) # From that, extract just main.xc:<line_no>
            assert n
            ref_main_line_num = n.group()
            ref_without_main_xc = re.sub(pattern, '', ref) # Remove (<path to main.xc>/main.xc:<line_no>) from the string so the rest can be compared
         else:
            assert False

         assert dut_without_main_xc == ref_without_main_xc # After removing the (<path_to_main.xc>/main.xc:<line_no>) part, everything else should match
         assert dut_main_line_num == ref_main_line_num # The main.xc:<line_no> bit should match

