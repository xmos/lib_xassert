# Copyright (c) 2015-2019, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('fail_test.expect'),
                                       'lib_xassert', 'simple_tests',
                                       'fail_test', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              'fail_test/bin/fail_test.xe',
                              tester=tester)
