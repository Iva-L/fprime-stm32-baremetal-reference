// ======================================================================
// \title  LedTestMain.cpp
// \author ivanlara
// \brief  cpp file for Led component test main function
// ======================================================================

#include "LedTester.hpp"

TEST(Nominal, TestBlinking) {
    LedBlinker::LedTester tester;
    tester.testBlinking();
}

TEST(Nominal, TestBlinkingOff) {
    LedBlinker::LedTester tester;
    tester.testBlinkingOff();
}

TEST(Nominal, TestBlinkingInterval) {
    LedBlinker::LedTester tester;
    tester.testBlinkInterval();
}

TEST(Nominal, TestInvalidParameter) {
    LedBlinker::LedTester tester;
    tester.testInvalidParameter();
}

TEST(Nominal, TestRunHandlerBranches) {
    LedBlinker::LedTester tester;
    tester.testRunHandlerBranches();
}

TEST(Nominal, TestBlinkingOffWhileOn) {
    LedBlinker::LedTester tester;
    tester.testBlinkingOffWhileOn();
}

TEST(Nominal, TestRunHandlerInvalidParam) {
    LedBlinker::LedTester tester;
    tester.testRunHandlerInvalidParam();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
