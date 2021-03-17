# Copyright 2015-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('assert_test.expect'),
                                       'lib_xassert', 'simple_tests',
                                       'assert_test', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              'assert_test/bin/assert_test.xe',
                              tester=tester)
