//
// Created by aren on 24/2/21.
//

#ifndef DIVECOMPUTER_UI_TESTS_H
#define DIVECOMPUTER_UI_TESTS_H

#include <catch2/catch_test_macros.hpp>
#include <UI/Menu.h>
#include <utility>
#include <UI/UIElement.h>
#include <UI/Screen.h>

std::string callbackReturn;
std::string mockReturn;
bool passParameter;
int counter; // A generic counter so we can assert that all blocks are hit in complex test cases with lots of callbacks

#endif //DIVECOMPUTER_UI_TESTS_H
