#ifndef DHWORKER_H
#define DHWORKER_H

#include <QObject>
#include <memory>
#include <semaphore>
#include <mutex>
#include <queue>
#include <vector>
#include "DH.h"

class DHWorker : public QObject
{
    Q_OBJECT
    std::unique_ptr<DH> DHA{};
    bool IsActive{true};
    std::queue<std::vector<float>> ThetasToCalc{};
    std::mutex MutexQueueThetas{};
    std::counting_semaphore<> SemQueueThetas{0};
    void GetFromQueue(std::vector<float>&);
    void PutToQueue(std::vector<float>&);
public:
    explicit DHWorker(QObject *parent = nullptr);

signals:
    void sendPos(const float& X, const float& Y, const float& Z);
    void configLoaded(const QVariantList& Thetas);

public slots:
    void finish();
    void workerFunc();
    void orderToCalc(const QVariantList& Thetas);
    void loadConfig(const QUrl& FilePath);
};

#endif // DHWORKER_H
