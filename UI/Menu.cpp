//
// Created by aren on 2/8/21.
//


#include "Menu.h"

MenuItem::MenuItem(std::string menuText, void (*const callback)(MenuItem &item)) {
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

std::string MenuItem::GetText() const {
    return this->MenuText;
}

MenuItem::MenuItem(MenuItem **existingItem) {
    this->MenuText = (*existingItem)->MenuText;
    this->Callback = (*existingItem)->Callback;
    this->Enabled = (*existingItem)->Enabled;
}

MenuItem::~MenuItem() {
}

Menu::Menu(std::vector<MenuItem> *items, void(*drawFunction)(std::string), void (*clearFunction)()) {
    this->Items = items;
    this->DrawFunction = drawFunction;
    this->ClearFunction = clearFunction;
    this->CurrIndex = 0;
}

Menu::~Menu() {
    this->Items->clear();
    delete this->Items;
}

void Menu::ShowMenu() {
    this->DrawFunction(this->Items->at(this->CurrIndex).GetText());
}

// Return 0 for normal scroll, return 1 if overflow
int Menu::IncrementMenu() {
    this->CurrIndex += 1;
    if (this->CurrIndex >= this->Items->size()) {
        CloseMenu();
        return 1;
    } else if (!this->Items->at(this->CurrIndex).CheckEnabled()) // If a menu item is disabled, skip it
    {
        IncrementMenu();
    } else {
        (this->ClearFunction)();
        this->ShowMenu();
    }
    return 0;
}

void Menu::SelectItem() {
    MenuItem currItem = this->Items->at(this->CurrIndex);
    currItem.Callback(currItem);
}

void Menu::CloseMenu() {
    this->CurrIndex = 0;
}

void Menu::AddItem(MenuItem item) {
    this->Items->push_back(item);
}

void Menu::ClearItems() {
    this->Items->clear();
}

int Menu::GetSize() {
    return this->Items->size();
}
