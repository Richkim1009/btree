add_test( FirstTest.BasicAssertions /home/polaris/study/cpp/btree/build/first_test [==[--gtest_filter=FirstTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( FirstTest.BasicAssertions PROPERTIES WORKING_DIRECTORY /home/polaris/study/cpp/btree/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( first_test_TESTS FirstTest.BasicAssertions)
