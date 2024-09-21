#include "dhworker.h"
#include <vector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDebug>
#include <QVariantList>
#include <QVariant>
#include <QVariantMap>

DHWorker::DHWorker(QObject *parent)
    : QObject{parent},
    DHA{std::make_unique<DH>()},
    IsActive{true}
{}

void DHWorker::GetFromQueue(std::vector<float>& Thetas)
{
    std::scoped_lock Lock(MutexQueueThetas);
    std::vector<float> Tmp{};
    if(!ThetasToCalc.empty())
    {
        Thetas = std::move(ThetasToCalc.front());
        ThetasToCalc.pop();
    }
}

void DHWorker::finish()
{
    IsActive = false;
    SemQueueThetas.release(999);
}

void DHWorker::workerFunc()
{
    while(IsActive)
    {
        SemQueueThetas.acquire();
        if(!IsActive)
        {
            return;
        }
        std::vector<float> Thetas;
        GetFromQueue(Thetas);
        DH::Pos XYZ = DHA->CalcLastJointPos(Thetas);
        emit sendPos(XYZ.x, XYZ.y, XYZ.z);
    }
}

void DHWorker::orderToCalc(const QVariantList& Thetas)
{
    std::scoped_lock Lock(MutexQueueThetas);
    std::vector<float> Tmp{};
    for(const auto& Theta : Thetas)
    {
        Tmp.emplace_back(Theta.toFloat());
    }
    ThetasToCalc.push(std::move(Tmp));
    SemQueueThetas.release(1);
}

void DHWorker::loadConfig(const QUrl& FilePath)
{
    QFile ConfigFile(FilePath.toLocalFile());
    if(ConfigFile.open(QFile::ReadOnly))
    {
        QJsonParseError JsonError{};
        QJsonDocument ConfigJson = QJsonDocument::fromJson(ConfigFile.readAll(), &JsonError);
        if(JsonError.error == QJsonParseError::NoError)
        {
            std::vector<DH::Joint> Joints;
            QVariantList Thetas{};
            for(const auto& Element : ConfigJson.array())
            {
                const auto& Obj = Element.toObject();
                QVariantMap ThetaMap{};
                ThetaMap.insert("value", Obj.value("Theta").toDouble());
                Thetas.append(ThetaMap);
                Joints.emplace_back(static_cast<float>(Obj.value("Theta").toDouble()),
                                    static_cast<float>(Obj.value("A").toDouble()),
                                    static_cast<float>(Obj.value("D").toDouble()),
                                    static_cast<float>(Obj.value("Alpha").toDouble())
                                    );
            }
            DHA->SetJoints(Joints);
            emit configLoaded(Thetas);
        }
        else
        {
            qDebug() << JsonError.errorString();
        }
    }
}
