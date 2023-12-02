add_executable(
  assembler_test
  # test files
  scanner_test.cc
  parser_test.cc
  # source files
  ../src/scanner.cpp
  ../src/parser.cpp
)
target_link_libraries(
  assembler_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(assembler_test)