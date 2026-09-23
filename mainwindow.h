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
    SensorDataReader m_reader;
    QtDataVisualization::Q3DScatter *m_scatterGraph;
    QWidget *m_containerWidget;
    void initializeVisualization();
    void setup3DGraph();
    void displayHeadTrajectory();
};
#endif // MAINWINDOW_H
