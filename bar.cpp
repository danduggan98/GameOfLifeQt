#include "bar.h"

//Bar constructor
Bar::Bar(float width, float height, QObject *parent) : QObject(parent) {
    width_ = width;
    height_ = height;
}
