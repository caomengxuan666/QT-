#include "mainwindow.h"
#include"QDir"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    QFile stylefile("mystyle.css");
    stylefile.open(QIODevice::ReadOnly);
    QString stylesheet=QString::fromLatin1(stylefile.readAll());
    a.setStyleSheet(stylesheet);
    return a.exec();
}
