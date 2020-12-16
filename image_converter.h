#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

class QImage;

namespace cv {
class Mat;
}

QImage cvMat_to_QImage(const cv::Mat &mat);

#endif // IMAGE_CONVERTER_H
