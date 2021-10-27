//
// Created by aren on 2/8/21.
//

#include "Menu.h"

MenuItem::MenuItem(const char *menuText, void (*const callback)(MenuItem &item)) {
    this->MenuText = menuText;
    this->Callback = callback;
    this->Enabled = true;
}

void MenuItem::Enable() {
    this->Enabled = true;
}

void MenuItem::Disable() {
    this->Enabled = false;
}

bool MenuItem::CheckEnabled() const {
    return Enabled;
}

const char *MenuItem::GetText() {
    return this->MenuText;
}

Menu::Menu(MenuItem **items, int itemsSize, int menuHeight) {
    this->Items = items;
    this->ItemsSize = itemsSize;
    this->MenuHeight = menuHeight;
    this->CurrIndex = 0;
}

void Menu::ShowMenu() {
    hardwareManager.tft.setFont(Terminal12x16);
    hardwareManager.tft.drawText(0, hardwareManager.tft.maxY() - this->MenuHeight,
                                 this->Items[this->CurrIndex]->GetText());
}

// Return 0 for normal scroll, return 1 if overflow
int Menu::IncrementMenu() {
    this->CurrIndex += 1;
    if (this->CurrIndex >= this->ItemsSize) {
        CloseMenu();
        return 1;
    } else if (!this->Items[this->CurrIndex]->CheckEnabled()) // If a menu item is disabled, skip it
    {
        IncrementMenu();
    } else {
        hardwareManager.tft.fillRectangle(0, hardwareManager.tft.maxY() - this->MenuHeight,
                                          hardwareManager.tft.maxX(), hardwareManager.tft.maxY(),
                                          COLOR_BLACK); // Clear prior menu item
        this->ShowMenu();
    }
    return 0;
}

void Menu::SelectItem() {
    MenuItem *currItem = this->Items[this->CurrIndex];
    currItem->Callback(*currItem);
}

void Menu::CloseMenu() {
    this->CurrIndex = 0;
}
