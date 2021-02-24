//
// Created by aren on 24/2/21.
//

#include <Tests/TestsManager.h>

int main(int argc, char *argv[]) {
    std::vector<Test> tests;

    RegisterDecoTests(tests);

    bool allPass = RunTests(tests);
    if (allPass) {
        return 0;
    } else {
        return 1;
    }
}

bool RunTests(std::vector<Test> tests) {
    std::cout << "Starting Testing:" << std::endl << std::endl;
    int testCount = 0;
    int testPass = 0;
    for (auto &test : tests) {
        testCount++;
        std::cout << test.Name << ": ";
        bool pass = test.UnitTest();
        int SpacesNeeded = ROW_COLUMN - test.Name.size();
        for (int i = 0; i < SpacesNeeded; i++) {
            std::cout << " ";
        }
        if (pass) {
            testPass++;
            std::cout << "PASS";
        } else {
            std::cout << "FAIL";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "All tests complete: " << testPass << "/" << testCount << std::endl;
    return testCount == testPass;
}