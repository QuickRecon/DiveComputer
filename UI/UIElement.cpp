//
// Created by aren on 2/8/21.
//

#include "UIElement.h"

std::string UIElement::GetTitle() {
    return this->Title;
}

std::string UIElement::GetData() {
    return this->Data;
}

void UIElement::UpdateData() {
    this->Data = this->UpdateFunction();
}

int UIElement::GetPosition() {
    return this->Position;
}

UIElement::UIElement(std::string title, int position, std::string (*updateFunction)(),
                     void (*drawFunction)(int, int, UIElement *)) {
    this->Title = std::move(title);
    this->Position = position;
    this->UpdateFunction = updateFunction;
    this->DrawFunction = drawFunction;
    this->Data = "";
}

void UIElement::Draw(int XCoord, int YCoord) {
    this->DrawFunction(XCoord, YCoord, this);
}
