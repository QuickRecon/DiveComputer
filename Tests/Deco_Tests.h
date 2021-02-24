//
// Created by aren on 24/2/21.
//

#ifndef DIVECOMPUTER_DECO_TESTS_H
#define DIVECOMPUTER_DECO_TESTS_H

#include "TestsManager.h"
#include <Deco.h>


void RegisterDecoTests(std::vector<Test> &tests);

bool TestAddGas();

bool TestMeterBar();

bool TestBarMeter();

bool TestSwitchGas();

#endif //DIVECOMPUTER_DECO_TESTS_H
