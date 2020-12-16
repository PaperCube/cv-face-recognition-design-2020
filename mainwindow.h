#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dnnfacedetect.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void on_btnCaptureImage_clicked();

    void on_btnDetectFace_clicked();

  private:
    Ui::MainWindow *ui;

    QImage cvMat_to_QImage(const cv::Mat &mat);
    VideoCapture cap;
    Mat frame;
    Mat pic;
    QImage image;
    bool mark;
    M checkR;
    int cameraMark;
};

#endif // MAINWINDOW_H
