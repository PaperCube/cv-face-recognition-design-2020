#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    wiringPiSetup();
    pinMode(7, OUTPUT);
    pinMode(0, OUTPUT);
    digitalWrite(7, LOW);
    digitalWrite(0, LOW);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
