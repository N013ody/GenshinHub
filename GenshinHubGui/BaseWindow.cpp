#include "BaseWindow.h"

void BaseWindow::setVisble(bool isVisble) {
    this->visble = isVisble;
}

void BaseWindow::setInit(bool init)
{
    this->init = init;
}

bool BaseWindow::isVisble() {
    return this->visble;
}

bool BaseWindow::isInit()
{
    return this->init;
}

float BaseWindow::getWidth() { return width; }
float BaseWindow::getHeight() { return height; }

void BaseWindow::setWidth(float width)
{
    this->width = width;
}

void BaseWindow::setHeight(float height)
{
    this->height = height;
}
