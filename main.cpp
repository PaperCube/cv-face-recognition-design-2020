#include "mainwindow.h"
#include "led_controller.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    led_controller::ensure_setup();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
