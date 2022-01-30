//
// Created by aren on 24/2/21.
//
#include <Utils/Constants.h>
#include "Deco_Tests.h"

TEST_CASE("Can add gas to deco model", "[Deco]") {
    double O2 = 0.32;
    double N2 = 0.6;
    double He = 0.08;
    Deco *testDeco = new Deco();
    int gasCountInitial = testDeco->Gases.size();
    testDeco->AddGas(N2, O2, He);
    int gasCountFinal = testDeco->Gases.size();

    REQUIRE(gasCountInitial + 1 == gasCountFinal);
    REQUIRE(testDeco->Gases[gasCountInitial].FrO2 == O2);
    REQUIRE(testDeco->Gases[gasCountInitial].FrN2 == N2);
    REQUIRE(testDeco->Gases[gasCountInitial].FrHe == He);

    delete testDeco;
}

TEST_CASE("Can switch gas", "[Deco]") {
    double O2 = 0.32;
    double N2 = 0.6;
    double He = 0.08;
    Deco *testDeco = new Deco();
    testDeco->AddGas(N2, O2, He);
    testDeco->SwitchGas(1);
    REQUIRE(testDeco->CurrentGas == 1);

    delete testDeco;
}

TEST_CASE("Can convert meter to bar", "[Deco]") {
    double bar = 2;
    double meter = 10;
    WaterDensity = 10.00;
    SurfacePressure = 1;
    REQUIRE(MeterToBar(meter) - bar < FLOAT_COMPARE_TOLERANCE);
}

TEST_CASE("Can convert bar to meter", "[Deco]") {
    double bar = 2;
    double meter = 10;
    WaterDensity = 10.00;
    SurfacePressure = 1;
    REQUIRE(BarToMeter(bar) - meter < FLOAT_COMPARE_TOLERANCE);
}
