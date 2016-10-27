import xmostest

def runtest():
    test_name = "assert_test_unit"
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open(test_name + '.expect'),
                                       'lib_xassert', 'simple_tests',
                                       test_name, {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              test_name + '/bin/' + test_name + '.xe',
                              tester=tester)
