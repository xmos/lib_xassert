# Copyright 2015-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('unreachable_test.expect'),
                                       'lib_xassert', 'simple_tests',
                                       'unreachable_test', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              'unreachable_test/bin/unreachable_test.xe',
                              tester=tester)
