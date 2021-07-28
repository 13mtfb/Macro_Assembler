#include "../src/scanner.h"
//#include "../src/scanner.cpp"
#include "../src/tokens.h"
#include <string>
#include "iostream"
#include <fstream>

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(ScannerTest, validateTokens) {

    string filename = "../../examples/example_1.txt";


    scanner Scanner(filename);
    EXPECT_EQ(Scanner.scan(), pSymbol) << "Should expect pSymbol as the first token";
}