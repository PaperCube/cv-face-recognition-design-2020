#ifndef DNNFACEDETECT_H
#define DNNFACEDETECT_H

#include <QFileDialog>
#include <QLabel>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/time.h>
#include <wiringPi.h>
using namespace std;
using namespace cv;
struct M {
    Mat frameR;
    bool markR;
};
class dnnfacedetect {
  private:
    string _modelbinary, _modeldesc;
    dnn::Net _net;

  public:
    //构造函数 传入模型文件
    dnnfacedetect();
    dnnfacedetect(string modelBinary, string modelDesc);
    ~dnnfacedetect();
    //置信阈值
    float confidenceThreshold;
    double inScaleFactor;
    size_t inWidth;
    size_t inHeight;
    Scalar meanVal;
    //初始化DNN网络
    bool initdnnNet();
    //人脸检测
    // vector<Mat> detect(Mat frame);
    M detect(Mat frame);
};
#endif // DNNFACEDETECT_H
