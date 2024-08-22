
add_executable(${PROJECT_NAME}
    suites/test_timer.cpp
    suites/test_timer_set_interval.cpp
    suites/test_timer_set_timeout.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE timer gtest gtest_main gmock)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}
    XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_report/xml
)
