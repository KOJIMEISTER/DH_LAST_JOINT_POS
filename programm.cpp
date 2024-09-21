#include "programm.h"

Programm::Programm(QObject *parent) :
    QObject{parent},
    Back{std::make_unique<Backend>()},
    DHA{std::make_unique<DHA>()}
{
    connect();
}

Backend* Programm::GetBackend()
{
    return Back.get();
}

Programm::~Programm()
{

}
