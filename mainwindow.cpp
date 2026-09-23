#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QScatter3DSeries>
#include <QSlider>
#include <QLayout>
#include <QVBoxLayout>
#include <QTimer>



using namespace QtDataVisualization;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if (ui->centralwidget->layout() == nullptr) {
        new QVBoxLayout(ui->centralwidget);
    }

    QTimer::singleShot(100, this, &MainWindow::initializeVisualization);
}

void MainWindow::initializeVisualization()
{

    m_scatterGraph = new Q3DScatter();
    m_containerWidget = QWidget::createWindowContainer(m_scatterGraph);

    Q3DTheme *theme = m_scatterGraph->activeTheme();
    theme->setType(Q3DTheme::ThemeEbony);


    theme->setBackgroundColor(QColor(40, 40, 40));
    theme->setWindowColor(QColor(40, 40, 40));


    theme->setGridLineColor(QColor(80, 80, 80));
    theme->setLabelTextColor(Qt::black);

    m_scatterGraph->axisX()->setRange(-2.0f, 2.0f);
    m_scatterGraph->axisY()->setRange(-2.0f, 2.0f);
    m_scatterGraph->axisZ()->setRange(-2.0f, 2.0f);

    m_scatterGraph->axisX()->setTitle("X (Влево/Вправо)");
    m_scatterGraph->axisY()->setTitle("Y (Высота)");
    m_scatterGraph->axisZ()->setTitle("Z (Вперед/Назад)");


    ui->centralwidget->layout()->addWidget(m_containerWidget);



    QString filePath = ":/data/trackingData_20260505_165740.txt";

    if (m_reader.loadTrackingData(filePath)) {
        const auto& frames = m_reader.getBodyFrames();
        if (frames.isEmpty()) return;


        QScatter3DSeries *bodySeries = new QScatter3DSeries();
        bodySeries->setMesh(QAbstract3DSeries::MeshSphere);
        bodySeries->setItemSize(0.15f);     // 15 cm
        bodySeries->setBaseColor(Qt::red);
        bodySeries->setMeshSmooth(true);
        m_scatterGraph->addSeries(bodySeries);


        static int currentFrameIndex = 0;


        QTimer *playbackTimer = new QTimer(this);

        connect(playbackTimer, &QTimer::timeout, this, [this, bodySeries, frames]() {
            if (frames.isEmpty()) return;


            const auto& currentFrame = frames[currentFrameIndex];

            if (!currentFrame.joints.isEmpty()) {
                QScatterDataArray *dataArray = new QScatterDataArray;
                dataArray->resize(currentFrame.joints.size());

                for (int i = 0; i < currentFrame.joints.size(); ++i) {
                    (*dataArray)[i].setPosition(currentFrame.joints[i]);
                }


                bodySeries->dataProxy()->resetArray(dataArray);
            }


            currentFrameIndex++;


            if (currentFrameIndex >= frames.size()) {
                currentFrameIndex = 0;
            }
        });


        playbackTimer->start(15);
    }
}

void MainWindow::setup3DGraph()
{
    m_scatterGraph = new Q3DScatter();


    m_containerWidget = QWidget::createWindowContainer(m_scatterGraph);


    m_scatterGraph->axisX()->setTitle("Ось X (Метры)");
    m_scatterGraph->axisY()->setTitle("Ось Y (Вертикаль)");
    m_scatterGraph->axisZ()->setTitle("Ось Z (Метры)");


    m_scatterGraph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_scatterGraph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);


    if (ui->centralwidget->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(layout);
    }
    ui->centralwidget->layout()->addWidget(m_containerWidget);
}

void MainWindow::displayHeadTrajectory()
{

    const QVector<QVector3D>& headData = m_reader.getHeadTrajectory();

    if (headData.isEmpty()) return;


    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(headData.size());


    for (int i = 0; i < headData.size(); ++i) {

        (*dataArray)[i].setPosition(headData[i]);
    }


    QScatter3DSeries *series = new QScatter3DSeries();
    series->dataProxy()->resetArray(dataArray);


    series->setMesh(QAbstract3DSeries::MeshSphere);
    series->setItemSize(0.05f);
    series->setMeshSmooth(true);


    m_scatterGraph->addSeries(series);

}

MainWindow::~MainWindow()
{
    delete ui;
}