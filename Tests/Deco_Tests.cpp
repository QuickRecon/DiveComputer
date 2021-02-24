//
// Created by aren on 24/2/21.
//
#include "Deco_Tests.h"

void RegisterDecoTests(std::vector<Test> &tests) {
    int index = tests.size();
    // Add Gas
    tests.emplace_back();
    tests[index].Name = "Add Gas";
    tests[index].UnitTest = &TestAddGas;
    index++;

    //  Meter to Bar
    tests.emplace_back();
    tests[index].Name = "Meter To Bar";
    tests[index].UnitTest = &TestMeterBar;
    index++;

    //  Bar to Meter
    tests.emplace_back();
    tests[index].Name = "Bar To Meter";
    tests[index].UnitTest = &TestBarMeter;
    index++;

    //  Switch Gas
    tests.emplace_back();
    tests[index].Name = "Switch Gas";
    tests[index].UnitTest = &TestSwitchGas;
    index++;
}

bool TestAddGas() {
    double O2 = 0.32;
    double N2 = 0.6;
    double He = 0.08;
    Deco *testDeco = new Deco();
    int gasCountInitial = testDeco->Gases.size();
    testDeco->AddGas(N2, O2, He);
    int gasCountFinal = testDeco->Gases.size();

    return gasCountInitial + 1 == gasCountFinal
           && testDeco->Gases[gasCountInitial].FrO2 == O2
           && testDeco->Gases[gasCountInitial].FrN2 == N2
           && testDeco->Gases[gasCountInitial].FrHe == He;
}

bool TestSwitchGas() {
    double O2 = 0.32;
    double N2 = 0.6;
    double He = 0.08;
    Deco *testDeco = new Deco();
    testDeco->AddGas(N2, O2, He);
    testDeco->SwitchGas(1);
    return testDeco->CurrentGas == 1;
}

bool TestMeterBar() {
    double bar = 2;
    double meter = 10;
    WaterDensity = 10.00;
    SurfacePressure = 1;
    return MeterToBar(meter) - bar < FLOAT_COMPARE_TOLERANCE;
}

bool TestBarMeter() {
    double bar = 2;
    double meter = 10;
    WaterDensity = 10.00;
    SurfacePressure = 1;
    return BarToMeter(bar) - meter < FLOAT_COMPARE_TOLERANCE;
}