#include "mainwindow.h"

#include <QApplication>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString filter = "All file(*.txt)";
    QString filename = QFileDialog::getOpenFileName(nullptr,"Choose file with data",QDir::homePath(),filter);
    if(!filename.isEmpty()){
        MainWindow w(filename);
        w.show();
        return QCoreApplication::exec();
    }

    return QCoreApplication::exec();
}
