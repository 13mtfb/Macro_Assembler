#include "../src/scanner.h"
#include "../src/tokens.h"
#include <string>
#include "iostream"
#include <fstream>
#include <vector>
#include <gtest/gtest.h>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

//Assert tokens generated by scanner match pre-generated results
TEST(ScannerTest, testScannerFiles)
{

    // define files under test
    const vector<string> testFiles = {
        "example_1",
        "example_2",
        "scanner"};

    string testFile, resultsFile;
    string testSymbol, resultsSymbol;

    int line, symbol = 0;

    fstream file;

    for (int i = 0; i < testFiles.size(); i++)
    {

        string testFile = "../../examples/" + testFiles[i] + ".txt";
        string resultsFile = "../../examples/scanner_output/scanner_" + testFiles[i] + ".txt";

        line = 1;

        file.open(resultsFile, fstream::in);
        if (!file.is_open())
        {
            FAIL() << "Cannot open results file: " << resultsFile;
        }

        scanner Scanner(testFile);
        testSymbol = "";

        GTEST_COUT << "Testing file: " << testFile << std::endl;

        while (testSymbol != scannerTokensASCII[pEOF])
        {

            testSymbol = scannerTokensASCII[Scanner.scan()];
            getline(file, resultsSymbol);

            ASSERT_EQ(testSymbol, resultsSymbol) << "mismatch on line number: " << line;
            line++;
            symbol++;
        }

        file.close();
    }

    GTEST_COUT << "Total symbols passed: " << symbol << std::endl;
}
