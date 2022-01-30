//
// Created by aren on 30/1/22.
//

#ifndef DIVECOMPUTER_DRAWER_H
#define DIVECOMPUTER_DRAWER_H


#include <string>

class Drawer {
public:
    void DrawMenu(std::string menuText);

    void ClearMenu();

private:
    int MenuHeight;
};


#endif //DIVECOMPUTER_DRAWER_H
