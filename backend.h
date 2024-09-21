#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUrl>
#include <QVariantList>

class Backend : public QObject
{
    Q_OBJECT
    QVariantList XYZ{0.f, 0.f, 0.f};
    Q_PROPERTY(QVariantList xyz MEMBER XYZ NOTIFY xyzChanged)
public:
    explicit Backend(QObject *parent = nullptr);

public slots:
    void getPos(const float& X, const float& Y, const float& Z);

signals:
    void loadConfig(const QUrl& FileUrl);
    void orderToCalc(const QVariantList& Thetas);
    void configChanged(const QVariantList& Thetas);
    void xyzChanged();
};

#endif // BACKEND_H
