#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "BaseFilter.h"
class OpenCVFilter : public BaseFilter  {
public:
    virtual cv::Mat process(const cv::Mat& img, int n, int m) override {
        if (n != m) {
            throw "OpenCV Filter supports only square kernels";
        }
        cv::Mat result(img.rows, img.cols, img.type());
        medianBlur(img, result, n);
        return result;
    }
};