//
// Created by aren on 24/2/21.
//

#include <cstring>
#include <catch2/matchers/catch_matchers.hpp>
#include "UI_Tests.h"

//region Helper Functions

Menu *MakeBasicMenu() {
    auto *item1 = new MenuItem("item 1", [](MenuItem &a) { callbackReturn = "item1"; });
    auto *item2 = new MenuItem("item 2", [](MenuItem &a) { callbackReturn = "item2"; });
    auto *item3 = new MenuItem("item 3", [](MenuItem &a) { callbackReturn = "item3"; });

    auto menuItems = new std::vector<MenuItem>();
    menuItems->push_back(&item1);
    menuItems->push_back(&item2);
    menuItems->push_back(&item3);

    delete item1;
    delete item2;
    delete item3;

    auto menu = new Menu(menuItems, [](std::string menuText) { callbackReturn = std::move(menuText); },
                         [] { callbackReturn = "clear"; });
    return menu;
}

std::string mock_update() {
    return mockReturn;
}
//endregion
//region Menu Tests
TEST_CASE("Can Disable/Enable Menu Item", "[UI]") {
    auto *item1 = new MenuItem("item 1", [](MenuItem &a) { callbackReturn = "item1"; });
    REQUIRE(item1->CheckEnabled());
    item1->Disable();
    REQUIRE(!item1->CheckEnabled());
    item1->Enable();
    REQUIRE(item1->CheckEnabled());
    delete item1;
}

TEST_CASE("Menu Item Text Is Correct", "[UI]") {
    auto *item1 = new MenuItem("item 1", [](MenuItem &a) { callbackReturn = "item1"; });
    REQUIRE(item1->GetText() == "item 1");
    delete item1;
}

TEST_CASE("Menu Size is Correct", "[UI]") {
    Menu *menu = MakeBasicMenu();
    REQUIRE(menu->GetSize() == 3);
    delete (menu);
}


TEST_CASE("Menu Callback Fires", "[UI]") {
    Menu *menu = MakeBasicMenu();
    menu->SelectItem();
    REQUIRE(callbackReturn == "item1");
    delete (menu);
}

TEST_CASE("Menu Increment Works", "[UI]") {
    Menu *menu = MakeBasicMenu();
    REQUIRE(menu->IncrementMenu() == 0);
    menu->SelectItem();
    REQUIRE(callbackReturn == "item2");
    delete (menu);
}

TEST_CASE("Close Menu Resets Location", "[UI]") {
    Menu *menu = MakeBasicMenu();
    REQUIRE(menu->IncrementMenu() == 0);
    menu->CloseMenu();
    menu->SelectItem();
    REQUIRE(callbackReturn == "item1");
    delete (menu);
}

TEST_CASE("Menu Overflows Correctly", "[UI]") {
    Menu *menu = MakeBasicMenu();
    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 1);

    menu->SelectItem();
    REQUIRE(callbackReturn == "item1");
    delete (menu);
}

TEST_CASE("Can Add Item", "[UI]") {
    Menu *menu = MakeBasicMenu();
    auto *item4 = new MenuItem("item 4", [](MenuItem &a) { callbackReturn = "item4"; });
    menu->AddItem(*item4);
    delete item4;
    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 0);

    menu->SelectItem();
    REQUIRE(callbackReturn == "item4");
    delete (menu);
}

TEST_CASE("Can Skip Disabled Item", "[UI]") {
    Menu *menu = MakeBasicMenu();
    auto *item4 = new MenuItem("item 4", [](MenuItem &a) { callbackReturn = "item4"; });
    auto *item5 = new MenuItem("item 5", [](MenuItem &a) { callbackReturn = "item5"; });
    item4->Disable();
    menu->AddItem(*item4);
    menu->AddItem(*item5);
    delete item4;
    delete item5;

    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 0);
    REQUIRE(menu->IncrementMenu() == 0);

    menu->SelectItem();
    REQUIRE(callbackReturn == "item5");
    delete (menu);
}
//endregion
//region UIElement Tests
TEST_CASE("Can retrieve element title", "[UI]") {
    auto *uiElement = new UIElement("Test Element", 0, mock_update, [](int, int, UIElement *) {});
    REQUIRE(uiElement->GetTitle() == "Test Element");
    delete uiElement;
}

TEST_CASE("Can retrieve element position", "[UI]") {
    auto *uiElement = new UIElement("Test Element", 3, mock_update, [](int, int, UIElement *) {});
    REQUIRE(uiElement->GetPosition() == 3);
    delete uiElement;
}

TEST_CASE("UI Element is updated", "[UI]") {
    auto *uiElement = new UIElement("Test Element", 0, mock_update, [](int, int, UIElement *) {});
    mockReturn = "test1";
    uiElement->UpdateData();
    REQUIRE(uiElement->GetData() == "test1");
    mockReturn = "test2";
    REQUIRE(uiElement->GetData() == "test1");
    uiElement->UpdateData();
    REQUIRE(uiElement->GetData() == "test2");
    delete uiElement;
}

TEST_CASE("UI Element draw is passed correct Params", "[UI]") {
    auto mock_draw = [](int x, int y, UIElement *ui) {
        REQUIRE(x == 10);
        REQUIRE(y == 20);
        REQUIRE(ui->GetTitle() == "Test Element");
        passParameter = true;
    };
    passParameter = false; // Basically use a globally scoped boolean to make sure that the test actually runs
    auto *uiElement = new UIElement("Test Element", 0, mock_update, mock_draw);

    uiElement->Draw(10, 20);
    REQUIRE(passParameter);
    delete uiElement;
}
//endregion
//region Screen Tests
TEST_CASE("Screen throws exception on mismatched dimension", "[UI]") {
    // Yes there is a memory leak here, too bad
    REQUIRE_THROWS_WITH(new Screen(new std::vector<UIElement>, 3, 3, 300, 300),
                        "Elements does not match screen dimensions");
}

TEST_CASE("Screen updates all elements", "[UI]") {
    counter = 0;

    auto uiElement1 = new UIElement("Element 1", 0, []() { return std::string("Element1"); },
                                    [](int x, int y, UIElement *ui) {
                                        REQUIRE(x == 0);
                                        REQUIRE(y == 0);
                                        REQUIRE(ui->GetData() == "Element1");
                                        counter++;
                                    });
    auto uiElement2 = new UIElement("Element 2", 1, []() { return std::string("Element2"); },
                                    [](int x, int y, UIElement *ui) {
                                        REQUIRE(x == 100);
                                        REQUIRE(y == 0);
                                        REQUIRE(ui->GetData() == "Element2");
                                        counter++;
                                    });
    auto uiElement3 = new UIElement("Element 3", 2, []() { return std::string("Element3"); },
                                    [](int x, int y, UIElement *ui) {
                                        REQUIRE(x == 0);
                                        REQUIRE(y == 100);
                                        REQUIRE(ui->GetData() == "Element3");
                                        counter++;
                                    });
    auto uiElement4 = new UIElement("Element 4", 3, []() { return std::string("Element4"); },
                                    [](int x, int y, UIElement *ui) {
                                        REQUIRE(x == 100);
                                        REQUIRE(y == 100);
                                        REQUIRE(ui->GetData() == "Element4");
                                        counter++;
                                    });

    auto *elements = new std::vector<UIElement>();
    elements->push_back(*uiElement1);
    elements->push_back(*uiElement2);
    elements->push_back(*uiElement3);
    elements->push_back(*uiElement4);

    auto *screen = new Screen(elements, 2, 2, 200, 200);
    screen->update();
    REQUIRE(counter == 4);
}
//endregion