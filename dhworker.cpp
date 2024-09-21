#include "dhworker.h"
#include <vector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDebug>

DHWorker::DHWorker(QObject *parent)
    : QObject{parent},
    DHA{std::make_unique<DH>()}
{}

void DHWorker::loadConfig(const QUrl& FilePath)
{
    QFile ConfigFile(FilePath);
    if(ConfigFile.open(QFile::ReadOnly))
    {
        QJsonParseError JsonError{};
        QJsonDocument ConfigJson = QJsonDocument::fromJson(ConfigFile.readAll(), &JsonError);
        if(JsonError.error == QJsonParseError::NoError)
        {
            std::vector<float> Joints;
            for(const auto& Element : ConfigJson.array())
            {
                const auto& Obj = Element.toObject();
                Joints.emplace_back(0.f,
                                    static_cast<float>(Obj.value("A").toDouble()),
                                    static_cast<float>(Obj.value("D").toDouble()),
                                    static_cast<float>(Obj.value("Alpha").toDouble())
                                    );
            }
            DHA->SetJoints(Joints);
        }
        else
        {
            qDebug() << JsonError.errorString();
        }
    }
}
