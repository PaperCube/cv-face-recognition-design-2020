#include "dnnfacedetect.h"
dnnfacedetect::dnnfacedetect() { dnnfacedetect("", ""); }
//构造函数
dnnfacedetect::dnnfacedetect(string modelBinary, string modelDesc) {
    _modelbinary = modelBinary;
    _modeldesc = modelDesc;
    //初始化置信阈值
    confidenceThreshold = 0.6;
    inScaleFactor = 0.5;
    inWidth = 300;
    inHeight = 300;
    meanVal = Scalar(104.0, 177.0, 123.0);
}
dnnfacedetect::~dnnfacedetect() { _net.~Net(); }
//初始化dnnnet
bool dnnfacedetect::initdnnNet() {
    _net = dnn::readNetFromTensorflow(_modelbinary, _modeldesc);
    _net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    _net.setPreferableTarget(dnn::DNN_TARGET_CPU);
    return !_net.empty();
}
//人脸检测
M dnnfacedetect::detect(Mat frame) {
    Mat tmpsrc = frame;
    M reu;
    reu.markR = false;
    vector<Mat> dsts = vector<Mat>();
    // 修改通道数
    if (tmpsrc.channels() == 4)
        cvtColor(tmpsrc, tmpsrc, COLOR_BGRA2BGR);
    // 输入数据调整
    Mat inputBlob = dnn::blobFromImage(
        tmpsrc, inScaleFactor, Size(inWidth, inHeight), meanVal, false, false);
    _net.setInput(inputBlob, "data");
    //人脸检测
    Mat detection = _net.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                     detection.ptr<float>());
    //检测出的结果进行绘制和存放到dsts中
    for (int i = 0; i < detectionMat.rows; i++) {
        //置值度获取
        float confidence = detectionMat.at<float>(i, 2);
        //如果大于阈值说明检测到人脸
        if (confidence > confidenceThreshold) {
            reu.markR = true;
            //计算矩形
            int xLeftBottom =
                static_cast<int>(detectionMat.at<float>(i, 3) * tmpsrc.cols);
            int yLeftBottom =
                static_cast<int>(detectionMat.at<float>(i, 4) * tmpsrc.rows);
            int xRightTop =
                static_cast<int>(detectionMat.at<float>(i, 5) * tmpsrc.cols);
            int yRightTop =
                static_cast<int>(detectionMat.at<float>(i, 6) * tmpsrc.rows);
            //生成矩形
            Rect rect((int)xLeftBottom, (int)yLeftBottom,
                      (int)(xRightTop - xLeftBottom),
                      (int)(yRightTop - yLeftBottom));
            //截出图矩形存放到dsts数组中
            Mat tmp = tmpsrc(rect);
            dsts.push_back(tmp);
            //在原图上用红框画出矩形
            rectangle(frame, rect, Scalar(0, 0, 255));
        }
    }
    cv::resize(frame, reu.frameR, Size(640, 480), 0, 0, INTER_LINEAR);
    return reu;
}
