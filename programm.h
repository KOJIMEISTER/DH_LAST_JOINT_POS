#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QObject>
#include <QThread>
#include <memory>
#include "backend.h"
#include "dhworker.h"

class Programm : public QObject
{
    Q_OBJECT
    std::unique_ptr<Backend> Back;
    std::unique_ptr<DHWorker> DHW;
    QThread DHWThread{this};
public:
    Programm(QObject *parent = nullptr);
    ~Programm();
    Programm(const Programm&) = delete;
    Backend* GetBackend();
};

#endif // PROGRAMM_H
