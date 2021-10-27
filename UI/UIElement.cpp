//
// Created by aren on 2/8/21.
//

#include "UIElement.h"

char *UIElement::GetTitle() {
    return const_cast<char *>(this->Title);
}

int UIElement::GetPosition() {
    return this->Position;
}

char *UIElement::GetData() {
    return this->Data;
}

void UIElement::UpdateData() {
    this->Data = this->UpdateFunction();
}

UIElement::UIElement(char *title, int position, char *(*updateFunction)()) {
    this->Title = title;
    this->Position = position;
    this->UpdateFunction = updateFunction;
    this->Data = "";
}
