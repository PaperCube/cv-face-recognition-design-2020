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

struct M {
    cv::Mat frameR;
    int target_cnt = 0;
};

using std::string;

using cv::Mat;
namespace dnn = cv::dnn;
using cv::Scalar;

class dnnfacedetect {
  private:
    std::string _modelbinary, _modeldesc;
    cv::dnn::Net _net;

  public:
    //构造函数 传入模型文件
    dnnfacedetect();
    dnnfacedetect(std::string modelBinary, std::string modelDesc);
    ~dnnfacedetect();
    //置信阈值
    float confidenceThreshold;
    double inScaleFactor;
    size_t inWidth;
    size_t inHeight;
    cv::Scalar meanVal;
    //初始化DNN网络
    bool initdnnNet();
    //人脸检测
    // vector<Mat> detect(Mat frame);
    std::vector<cv::Rect> detect(cv::Mat);
};
#endif // DNNFACEDETECT_H
