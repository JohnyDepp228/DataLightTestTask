#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization/q3dscatter.h>
#include "SensorDataReader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    Ui::MainWindow *ui;
    SensorDataReader m_reader;                 // Наш парсер данных
    QtDataVisualization::Q3DScatter *m_scatterGraph;                // Сам 3D-график
    QWidget *m_containerWidget;                // Виджет-контейнер для встраивания в окно
    void initializeVisualization();
    void setup3DGraph();                       // Метод настройки графика
    void displayHeadTrajectory();
};
#endif // MAINWINDOW_H
