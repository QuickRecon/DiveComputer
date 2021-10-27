//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_MENU_H
#define DIVECOMPUTER_MENU_H

#include "Hardware/HardwareManager.h"

class MenuItem {
public:
    MenuItem() = delete;

    MenuItem(const char *menuText, void (*callback)(MenuItem &item));

    void ( *Callback)(MenuItem &item){};

    void Enable();

    void Disable();

    bool CheckEnabled() const;

    const char *GetText();

private:
    const char *MenuText{};
    bool Enabled{};
};


class Menu {
public:
    Menu() = delete;

    Menu(MenuItem *(*items), int itemsSize, int menuHeight);

    void ShowMenu();

    int IncrementMenu();

    void SelectItem();

    void CloseMenu();

private:
    int MenuHeight;
    int ItemsSize;
    int CurrIndex;

    MenuItem *(*Items);
};


#endif //DIVECOMPUTER_MENU_H
