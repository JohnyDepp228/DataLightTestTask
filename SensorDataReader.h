#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QVector3D>


struct BodyFrame {
    qint64 timestamp;
    QVector<QVector3D> joints;
};

class SensorDataReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorDataReader(QObject *parent = nullptr);


    bool loadTrackingData(const QString &filePath);


    const QVector<QVector3D>& getHeadTrajectory() const { return m_headTrajectory; }
    const QVector<BodyFrame>& getBodyFrames() const { return m_bodyFrames; }

private:

    void readDataLine(const QString &sensorData);

    QVector<QVector3D> m_headTrajectory;
    QVector<BodyFrame> m_bodyFrames;
};
