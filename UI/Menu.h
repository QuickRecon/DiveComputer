//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_MENU_H
#define DIVECOMPUTER_MENU_H

#include <vector>
#include <string>

class MenuItem {
public:
    MenuItem() = delete;

    MenuItem(std::string menuText, void (*callback)(MenuItem &item));

    MenuItem(MenuItem **pItem);

    ~MenuItem();

    void ( *Callback)(MenuItem &item){};

    void Enable();

    void Disable();

    bool CheckEnabled() const;

    std::string GetText() const;

private:
    std::string MenuText{};
    bool Enabled{};
};


class Menu {
public:
    Menu() = delete;

    Menu(std::vector<MenuItem> *items, void (*drawFunction)(std::string), void (*clearFunction)());

    ~Menu();

    void ShowMenu();

    int IncrementMenu();

    void SelectItem();

    void CloseMenu();

    int GetSize();

    void AddItem(MenuItem item);

    void ClearItems();

private:
    void (*DrawFunction)(std::string);

    void (*ClearFunction)();

    int CurrIndex;

    std::vector<MenuItem> *Items;
};


#endif //DIVECOMPUTER_MENU_H
