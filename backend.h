#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUrl>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

signals:
    void loadConfig(const QUrl& FileUrl);
};

#endif // BACKEND_H
