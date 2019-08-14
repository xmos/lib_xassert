# Copyright (c) 2015-2019, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('assert_test.expect'),
                                       'lib_xassert', 'simple_tests',
                                       'assert_test', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              'assert_test/bin/assert_test.xe',
                              tester=tester)
