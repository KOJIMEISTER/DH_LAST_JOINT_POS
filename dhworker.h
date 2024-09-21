#ifndef DHWORKER_H
#define DHWORKER_H

#include <QObject>
#include <memory>
#include "DH.h"

class DHWorker : public QObject
{
    Q_OBJECT
    std::unique_ptr<DH> DHA{};
public:
    explicit DHWorker(QObject *parent = nullptr);

signals:
    void sendPos(const float& X, const float& Y, const float& Z);

public slots:
    void orderToCalc(const QVariantList& Thetas);
    void loadConfig(const QUrl& FilePath);
};

#endif // DHWORKER_H
