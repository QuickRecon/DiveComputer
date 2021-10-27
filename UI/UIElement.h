//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_UIELEMENT_H
#define DIVECOMPUTER_UIELEMENT_H


class UIElement {
public:
    UIElement() = delete;

    UIElement(char *title, int position, char *(*UpdateFunction)());

    char *GetTitle();

    int GetPosition();

    char *GetData();

    void UpdateData();

private:
    const char *Title;
    int Position;
    char *Data;

    char *(*UpdateFunction)();
};


#endif //DIVECOMPUTER_UIELEMENT_H
