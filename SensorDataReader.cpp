#include "SensorDataReader.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

SensorDataReader::SensorDataReader(QObject *parent)
    : QObject{parent}
{
}


bool SensorDataReader::loadTrackingData(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Ошибка: Не удалось открыть файл для чтения:" << filePath;
        return false;
    }


    m_headTrajectory.clear();
    m_bodyFrames.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            readDataLine(line);
        }
    }

    file.close();
    qDebug() << "Чтение завершено!";
    qDebug() << "Успешно загружено траекторий головы:" << m_headTrajectory.size();
    qDebug() << "Успешно загружено кадров тела:" << m_bodyFrames.size();
    return true;
}


void SensorDataReader::readDataLine(const QString &sensorData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(sensorData.toUtf8());
    if (!jsonDoc.isObject()) return;

    QJsonObject jsonObj = jsonDoc.object();


    qint64 tmStampRemote = jsonObj.value("timeStampNs").toVariant().toLongLong();


    if (jsonObj.contains("Head")) {
        QJsonObject headObj = jsonObj.value("Head").toObject();


        float x = headObj.value("x").toDouble();
        float y = headObj.value("y").toDouble();
        float z = headObj.value("z").toDouble();

        m_headTrajectory.append(QVector3D(x, y, z));
    }


    if (jsonObj.contains("Body"))
    {
        BodyFrame currentFrame;
        currentFrame.timestamp = tmStampRemote;

        QJsonArray jointsArray = jsonObj.value("Body").toObject().value("joints").toArray();

        for (const QJsonValue &jointValue : jointsArray)
        {
            QJsonObject jointObj = jointValue.toObject();
            QString pValue = jointObj["p"].toString();


            QStringList pValues = pValue.split(",");


            if (pValues.size() >= 6)
            {

                QString xStr = pValues[0] + "." + pValues[1];
                QString yStr = pValues[2] + "." + pValues[3];
                QString zStr = pValues[4] + "." + pValues[5];

                float x = xStr.toFloat();
                float y = yStr.toFloat();
                float z = zStr.toFloat();

                currentFrame.joints.append(QVector3D(x, y, z));
            }
        }

        if (!currentFrame.joints.isEmpty()) {

            static bool firstLog = true;
            if (firstLog) {
                qDebug() << "ПРИМЕР РЕАЛЬНЫХ КООРДИНАТ СУСТАВА:"
                         << "X =" << currentFrame.joints[0].x()
                         << "Y =" << currentFrame.joints[0].y()
                         << "Z =" << currentFrame.joints[0].z();
                firstLog = false;
            }
        }
        if (currentFrame.joints.size() == 24) {
            m_bodyFrames.append(currentFrame);
        }
    }
}
