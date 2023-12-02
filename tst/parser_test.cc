#include "../src/parser.h"
#include "../src/tokens.h"
#include <string>
#include "iostream"
#include <fstream>
#include <vector>
#include <gtest/gtest.h>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"
#define GTEST_ERROR std::cerr << "[          ] [ ERROR ]"

TEST(parserTest, direct_assignment_literal)
{

    parser Parser;

    // TEST=5
    vector<int> test_lineTokens = {pSymbol, pEqual, pNumericLiteral, pNewLine};
    vector<string> test_compoundTokens = {"TEST", "5"};

    // expected results
    int expected_output = 05;

    try
    {
        Parser.parse(test_lineTokens, test_compoundTokens);
    }
    catch (errorType e)
    {
        GTEST_ERROR << "error during parsing: " << errorTokensASCII[e] << std::endl;
    }

    int actual_output = Parser.returnSymbol("TEST");

    ASSERT_EQ(expected_output, actual_output) << "output tokens are not of equal length";
}

TEST(parserTest, direct_assignment_expression)
{

    parser Parser;

    // TEST=5
    vector<int> test_lineTokens = {pSymbol, pEqual, pLeftBracket, pNumericLiteral, pPlus, pNumericLiteral, pRightBracket, pNewLine};
    vector<string> test_compoundTokens = {"TEST", "5", "3"};

    // expected results
    int expected_output = 010;

    try
    {
        Parser.parse(test_lineTokens, test_compoundTokens);
    }
    catch (errorType e)
    {
        GTEST_ERROR << "error during parsing: " << errorTokensASCII[e] << std::endl;
    }

    int actual_output = Parser.returnSymbol("TEST");

    ASSERT_EQ(expected_output, actual_output) << "output tokens are not of equal length";
}

TEST(parserTest, single_operand_register_mode)
{

    parser Parser;

    // TEST=5
    vector<int> test_lineTokens = {pSymbol, pSymbol, pNewLine};
    vector<string> test_compoundTokens = {"INC", "R1"};

    // expected results
    // location counter + assembled instruction
    vector<int> expected_output = {00, 05201};
    vector<int> actual_output;

    try
    {
        Parser.parse(test_lineTokens, test_compoundTokens);
        Parser.setPassTwo();
        actual_output = Parser.parse(test_lineTokens, test_compoundTokens);
    }
    catch (errorType e)
    {
        GTEST_ERROR << "error during parsing: " << errorTokensASCII[e] << std::endl;
    }

    ASSERT_EQ(expected_output.size(), actual_output.size()) << "output tokens are not of equal length";
    for (int i = 0; i < actual_output.size(); i++)
    {
        ASSERT_EQ(expected_output[i], actual_output[i]) << "assembled output does not match";
    }
}

TEST(parserTest, double_operand_register_mode)
{

    parser Parser;

    // TEST=5
    vector<int> test_lineTokens = {pSymbol, pSymbol, pOperandFieldSeperator, pSymbol, pNewLine};
    vector<string> test_compoundTokens = {"MOV", "R1", "R2"};

    // expected results
    // location counter + assembled instruction
    vector<int> expected_output = {00, 010102};
    vector<int> actual_output;

    try
    {
        Parser.parse(test_lineTokens, test_compoundTokens);
        Parser.setPassTwo();
        actual_output = Parser.parse(test_lineTokens, test_compoundTokens);
    }
    catch (errorType e)
    {
        GTEST_ERROR << "error during parsing: " << errorTokensASCII[e] << std::endl;
    }

    ASSERT_EQ(expected_output.size(), actual_output.size()) << "output tokens are not of equal length";
    for (int i = 0; i < actual_output.size(); i++)
    {
        ASSERT_EQ(expected_output[i], actual_output[i]) << "assembled output does not match";
    }
}

TEST(parserTest, forward_assignment_complext)
{

    // Tests the following
    //  - expression parsing
    //  - forward assignment
    //  - location counter

    parser Parser;

    // FOUR=TEST
    // INC <3+<FOUR/2>>(R2)
    // TEST=.
    vector<vector<int>> test_lineTokens = {
        {pSymbol, pEqual, pSymbol, pNewLine},
        {pSymbol, pLeftBracket, pNumericLiteral, pPlus, pLeftBracket, pSymbol, pDivide, pNumericLiteral, pRightBracket, pRightBracket, pLeftParen, pSymbol, pRightParen, pNewLine},
        {pSymbol, pEqual, pNumericLiteral, pNewLine}};
    vector<vector<string>> test_compoundTokens = {
        {"FOUR", "TEST"},
        {"INC", "3", "FOUR", "2", "R2"},
        {"TEST", "4"}};

    // expected results
    // location counter + assembled instruction
    vector<vector<int>> expected_output = {
        {},
        {00, 05262, 05},
        {}};
    vector<vector<int>> actual_output = {
        {},
        {},
        {},
    };

    try
    {
        for (int i = 0; i < test_lineTokens.size(); i++)
        {
            Parser.parse(test_lineTokens[i], test_compoundTokens[i]);
        }
        Parser.setPassTwo();

        for (int i = 0; i < test_lineTokens.size(); i++)
        {
            actual_output[i] = Parser.parse(test_lineTokens[i], test_compoundTokens[i]);
        }
    }
    catch (errorType e)
    {
        GTEST_ERROR << "error during parsing: " << errorTokensASCII[e] << std::endl;
    }

    for (int i = 0; i < actual_output.size(); i++)
    {
        ASSERT_EQ(expected_output[i].size(), actual_output[i].size()) << "output tokens are not of equal length";

        for (int j = 0; j < actual_output[i].size(); j++)
        {
            ASSERT_EQ(expected_output[i][j], actual_output[i][j]) << "assembled output does not match";
        }
    }
}