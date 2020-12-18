#include "dnnfacedetect.h"
dnnfacedetect::dnnfacedetect() { dnnfacedetect("", ""); }
//构造函数
dnnfacedetect::dnnfacedetect(std::string modelBinary, std::string modelDesc) {
    _modelbinary = modelBinary;
    _modeldesc = modelDesc;
    //初始化置信阈值
    confidenceThreshold = 0.6;
    inScaleFactor = 0.5;
    inWidth = 300;
    inHeight = 300;
    meanVal = cv::Scalar(104.0, 177.0, 123.0);
}
dnnfacedetect::~dnnfacedetect() {}
//初始化dnnnet
bool dnnfacedetect::initdnnNet() {
    _net = dnn::readNetFromTensorflow(_modelbinary, _modeldesc);
    _net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    _net.setPreferableTarget(dnn::DNN_TARGET_CPU);
    return !_net.empty();
}
//人脸检测
std::vector<cv::Rect> dnnfacedetect::detect(cv::Mat frame) {
    using namespace std;
    using namespace cv;
    std::vector<Rect> rectangles;
//    Mat tmpsrc = frame;
//    M reu;
//    vector<Mat> dsts = vector<Mat>();
    // 修改通道数
    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2BGR);
    // 输入数据调整
    Mat inputBlob = dnn::blobFromImage(
        frame, inScaleFactor, Size(inWidth, inHeight), meanVal, false, false);
    _net.setInput(inputBlob, "data");
    //人脸检测
    Mat detection = _net.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                     detection.ptr<float>());
    //检测出的结果 进行绘制和存放到dsts中
    for (int i = 0; i < detectionMat.rows; i++) {
        //置值度获取
        float confidence = detectionMat.at<float>(i, 2);
        //如果大于阈值说明检测到人脸
        if (confidence > confidenceThreshold) {
//            reu.target_cnt++;
            //计算矩形
            int xLeftBottom =
                static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int yLeftBottom =
                static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int xRightTop =
                static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int yRightTop =
                static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
            //生成矩形
            Rect rect((int)xLeftBottom, (int)yLeftBottom,
                      (int)(xRightTop - xLeftBottom),
                      (int)(yRightTop - yLeftBottom));
            rectangles.push_back(rect);
            //截出图矩形存放到dsts数组中
//            Mat tmp = tmpsrc(rect);
//            dsts.push_back(tmp);
            //在原图上用红框画出矩形
//            rectangle(frame, rect, Scalar(0, 0, 255));
        }
    }
//    cv::resize(frame, reu.frameR, Size(640, 480), 0, 0, INTER_LINEAR);
    return rectangles;
}
