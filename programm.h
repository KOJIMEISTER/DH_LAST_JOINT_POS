#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QObject>
#include <memory>
#include "backend.h"
#include "DH.h"

class Programm : public QObject
{
    Q_OBJECT
    std::unique_ptr<Backend> Back;
    std::unique_ptr<DH> DHA;
public:
    Programm(QObject *parent = nullptr);
    ~Programm();
    Programm(const Programm&) = delete;
    Backend* GetBackend();
};

#endif // PROGRAMM_H
