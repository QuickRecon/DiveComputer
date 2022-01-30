//
// Created by aren on 24/2/21.
//

#include "UI_Tests.h"

#include <utility>

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

TEST_CASE("Menu Initializes", "[UI]") {
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
