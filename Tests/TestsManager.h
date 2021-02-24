//
// Created by aren on 24/2/21.
//

#ifndef DIVECOMPUTER_TESTSMANAGER_H
#define DIVECOMPUTER_TESTSMANAGER_H

#include <stdlib.h>
#include <vector>
#include <iostream>

#define FLOAT_COMPARE_TOLERANCE 0.001
#define ROW_COLUMN 30

struct Test {
    std::string Name;

    bool (*UnitTest)();
};

#include "Deco_Tests.h"

bool RunTests(std::vector<Test> tests);

#endif //DIVECOMPUTER_TESTSMANAGER_H
