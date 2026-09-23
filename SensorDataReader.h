#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QVector3D>

// Структура для одного кадра скелета тела (24 сустава)
struct BodyFrame {
    qint64 timestamp;
    QVector<QVector3D> joints; // Сюда запишем 24 точки (X,Y,Z)
};

class SensorDataReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorDataReader(QObject *parent = nullptr);

    // Главный метод для запуска чтения всего файла
    bool loadTrackingData(const QString &filePath);

    // Доступ к прочитанным данным для графиков
    const QVector<QVector3D>& getHeadTrajectory() const { return m_headTrajectory; }
    const QVector<BodyFrame>& getBodyFrames() const { return m_bodyFrames; }

private:
    // Ваша переписанная функция для обработки одной строчки JSON
    void readDataLine(const QString &sensorData);

    // Сюда сохраняем результаты чтения
    QVector<QVector3D> m_headTrajectory;
    QVector<BodyFrame> m_bodyFrames;
};
