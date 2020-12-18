#include "dnnfacedetect.h"
#include <QMainWindow>

using namespace std;
using namespace cv;

QImage cvMat_to_QImage(const cv::Mat &mat) {
    switch (mat.type()) {
    // 8-bit, 4 channel
    case CV_8UC4: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step,
                     QImage::Format_RGB32);
        return image;
    }
        // 8-bit, 3 channel
    case CV_8UC3: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step,
                     QImage::Format_RGB888);
        return image.rgbSwapped();
    }
        // 8-bit, 1 channel
    case CV_8UC1: {
        static QVector<QRgb> sColorTable;
        // only create our color table once
        if (sColorTable.isEmpty()) {
            for (int i = 0; i < 256; ++i)
                sColorTable.push_back(qRgb(i, i, i));
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step,
                     QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image;
    }
    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n",
               mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

void render(Mat &mat, vector<Rect> rectangles){
    for(auto &r : rectangles){
        rectangle(mat, r, Scalar(0, 0, 255));
    }
}
