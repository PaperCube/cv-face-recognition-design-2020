#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

#include <opencv2/opencv.hpp>
#include <vector>

class QImage;

namespace cv {
class Mat;
}

QImage cvMat_to_QImage(const cv::Mat &mat);
void render(cv::Mat &mat, std::vector<cv::Rect> rectangles);

#endif // IMAGE_CONVERTER_H
