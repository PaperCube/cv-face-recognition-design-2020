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
    //    string ModelBinary = "/opt/model/opencv_face_detector_uint8.pb";
    //    string ModelDesc = "/opt/model/opencv_face_detector.pbtxt";
    //    if (mark == false) {
    //        return;
    //    }
    //    mark = false;
    //    destroyWindow("test");
    //    cap.release();
    //    //初始化

    //    if (!frame.empty()) {
    //        checkR = fdetect.detect(frame);
    //        if (checkR.markR == true) {
    //            led7.off();
    //            led0.on();
    //        }
    //        image = cvMat_to_QImage(checkR.frameR);
    //        ui->label->setPixmap(QPixmap::fromImage(image));
    //    }
    //    // waitKey(5);
}

///********************Mat转QImage函数*********************/
// QImage MainWindow::cvMat_to_QImage(const cv::Mat &mat) {
//    switch (mat.type()) {
//    // 8-bit, 4 channel
//    case CV_8UC4: {
//        QImage image(mat.data, mat.cols, mat.rows, mat.step,
//                     QImage::Format_RGB32);
//        return image;
//    }
//        // 8-bit, 3 channel
//    case CV_8UC3: {
//        QImage image(mat.data, mat.cols, mat.rows, mat.step,
//                     QImage::Format_RGB888);
//        return image.rgbSwapped();
//    }
//        // 8-bit, 1 channel
//    case CV_8UC1: {
//        static QVector<QRgb> sColorTable;
//        // only create our color table once
//        if (sColorTable.isEmpty()) {
//            for (int i = 0; i < 256; ++i)
//                sColorTable.push_back(qRgb(i, i, i));
//        }
//        QImage image(mat.data, mat.cols, mat.rows, mat.step,
//                     QImage::Format_Indexed8);
//        image.setColorTable(sColorTable);
//        return image;
//    }
//    default:
//        qDebug("Image format is not supported: depth=%d and %d channels\n",
//               mat.depth(), mat.channels());
//        break;
//    }
//    return QImage();
//}
