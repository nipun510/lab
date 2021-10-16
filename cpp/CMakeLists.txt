cmake_minimum_required(VERSION 3.10)

project(lab)
set(CMAKE_CXX_STANDARD 20)

add_executable(loggerTest      loggerTest.cc)
add_executable(toStringTest    toStringTest.cc)
add_executable(observerTest    observerTest.cc)
add_executable(detailTest      detailTest.cc)
add_executable(typetraitTest   typetraitTest.cc)

enable_testing()

#logger testing
add_test(NAME logger COMMAND loggerTest)
set_tests_properties(logger PROPERTIES PASS_REGULAR_EXPRESSION "info")
set_tests_properties(logger PROPERTIES PASS_REGULAR_EXPRESSION "warn")
set_tests_properties(logger PROPERTIES PASS_REGULAR_EXPRESSION "error")


#toString testing
add_test(NAME tostring COMMAND toStringTest)

#typtrait testing
add_test(NAME typetrait COMMAND typetraitTest)

#detail testing
add_test(NAME detail COMMAND detailTest)

#observer testing
add_test(NAME observer COMMAND observerTest)
