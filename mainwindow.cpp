#include "mainwindow.h"
#include "led_controller.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btnCaptureImage_clicked() {
    cameraMark = 10;

    dt.set_display_listener([this](const QImage &img) {
        ui->label->setPixmap(QPixmap::fromImage(img));
    });

    detector_thread::invocation_result result = dt.start(cameraMark);

    switch (result) {
    case detector_thread::OK:
        led0.off();
        led7.on();
        break;
    case detector_thread::CAMERA_ERROR:
        ui->label->setText("Camera Error");
        led7.flash(500, 2);
        break;
    case detector_thread::DNN_INITILIAZTION_FAILURE:
        ui->label->setText("初始化DNN人脸检测失败！");
        led7.flash(500, 2);
        break;
    default:
        ui->label->setText("Unknown error");
    }
    //    namedWindow("test");
}

void MainWindow::on_btnDetectFace_clicked() {
    // pause / resume
    detector_thread::State current_state = dt.state;
    if(current_state == detector_thread::RUNNING){
        dt.state = detector_thread::PAUSED;
    } else if(current_state == detector_thread::PAUSED){
        dt.state = detector_thread::RUNNING;
    }
}
