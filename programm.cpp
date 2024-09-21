#include "programm.h"
#include <QDebug>

Programm::Programm(QObject *parent) :
    QObject{parent},
    Back{std::make_unique<Backend>()},
    DHW{std::make_unique<DHWorker>()}
{
    DHW->moveToThread(&DHWThread);
    connect(Back.get(), &Backend::loadConfig, DHW.get(), &DHWorker::loadConfig, Qt::DirectConnection);
    connect(Back.get(), &Backend::orderToCalc, DHW.get(), &DHWorker::orderToCalc, Qt::DirectConnection);
    connect(DHW.get(), &DHWorker::configLoaded, Back.get(), &Backend::configChanged, Qt::DirectConnection);
    connect(DHW.get(), &DHWorker::sendPos, Back.get(), &Backend::getPos);
    connect(&DHWThread, &QThread::started, DHW.get(), &DHWorker::workerFunc);
    DHWThread.start();
}

Backend* Programm::GetBackend()
{
    return Back.get();
}

Programm::~Programm()
{
    DHW->finish();
    DHWThread.quit();
    DHWThread.wait();
}
