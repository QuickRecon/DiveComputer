//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_UIELEMENT_H
#define DIVECOMPUTER_UIELEMENT_H

#include <string>
#include <utility>

class UIElement {
public:
    UIElement() = delete;

    UIElement(std::string title, int position, std::string (*UpdateFunction)(),
              void (*DrawFunction)(int, int, UIElement *));

    int GetPosition();

    std::string GetTitle();

    std::string GetData();

    void UpdateData();

    void Draw(int XCoord, int YCoord);

private:
    std::string Title;
    std::string Data;
    int Position;

    std::string (*UpdateFunction)();

    void (*DrawFunction)(int, int, UIElement *);
};


#endif //DIVECOMPUTER_UIELEMENT_H
