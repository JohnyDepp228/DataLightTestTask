#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QScatter3DSeries>
#include <QSlider>
#include <QLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QSpacerItem>




using namespace QtDataVisualization;


MainWindow::MainWindow(QString filePath,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->filePath = filePath;
    Output();
}

void MainWindow::initializeVisualization()
{

    m_scatterGraph = new Q3DScatter();
    m_containerWidget = QWidget::createWindowContainer(m_scatterGraph);


    m_scatterGraph->axisX()->setTitleVisible(false);
    m_scatterGraph->axisY()->setTitleVisible(false);
    m_scatterGraph->axisZ()->setTitleVisible(false);


    Q3DTheme *theme = m_scatterGraph->activeTheme();
    theme->setType(Q3DTheme::ThemeEbony);


    theme->setBackgroundEnabled(false);
    theme->setGridEnabled(false);
    theme->setLabelBackgroundEnabled(false);

    QFont hiddenFont = theme->font();
    hiddenFont.setPointSize(0);
    theme->setFont(hiddenFont);
    theme->setLabelTextColor(QColor(0, 0, 0, 0));

    theme->setGridLineColor(QColor(0, 0, 0, 0));
    theme->setGridLineColor(QColor(0, 0, 0, 0));

    theme->setBackgroundColor(QColor(0, 0, 0));
    theme->setWindowColor(QColor(0, 0, 0));


    m_scatterGraph->axisX()->setRange(-2.0f, 2.0f);
    m_scatterGraph->axisY()->setRange(-2.0f, 2.0f);
    m_scatterGraph->axisZ()->setRange(-2.0f, 2.0f);

    ui->centralwidget->layout()->addWidget(m_containerWidget);
    QScatter3DSeries *roomFloorSeries = new QScatter3DSeries();
    QScatterDataArray *floorData = new QScatterDataArray;

    float floorY = -1.8f;
    for (float x = -2.0f; x <= 2.0f; x += 0.15f) {
        for (float z = -2.0f; z <= 2.0f; z += 0.15f) {
            floorData->append(QScatterDataItem(QVector3D(x, floorY, z)));
        }
    }
    roomFloorSeries->dataProxy()->resetArray(floorData);
    roomFloorSeries->setMesh(QAbstract3DSeries::MeshBar);
    roomFloorSeries->setItemSize(0.3f);
    roomFloorSeries->setBaseColor(QColor(80, 80, 80));
    m_scatterGraph->addSeries(roomFloorSeries);

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


        playbackTimer->start(10);
    }
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

void MainWindow::Output()
{
    if (ui->centralwidget->layout() == nullptr) {
        new QVBoxLayout(ui->centralwidget);
    }
    QTimer::singleShot(50, this, &MainWindow::initializeVisualization);

}

MainWindow::~MainWindow()
{
    delete ui;
}

