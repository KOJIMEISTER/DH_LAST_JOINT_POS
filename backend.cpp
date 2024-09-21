#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{}

void Backend::getPos(const float& X, const float& Y, const float& Z)
{
    XYZ[0] = X;
    XYZ[1] = Y;
    XYZ[2] = Z;
    emit xyzChanged();
}
