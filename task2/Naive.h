#include <opencv2/opencv.hpp>
#include <iostream>
#include "BaseFilter.h"
class NaiveFilter : public BaseFilter  {
public:

    uchar getPixel(const cv::Mat& img, int channel, int n, int m, int x, int y) {
        std::vector<uchar> pixels;
        pixels.reserve(n * m);
        for(size_t i = x - n / 2; i <= x + n / 2; ++i) {
            for(size_t j = y - m / 2; j <= y + m / 2; ++j) {
                pixels.emplace_back(img.at<cv::Vec3b>(i, j)[channel]);
            }
        }
        std::sort(pixels.begin(), pixels.end());
        return pixels[pixels.size() / 2];
    }

    virtual void processChannel(const cv::Mat& img, cv::Mat& result, int n, int m, int channel) override {
        for(size_t i = n / 2; i < img.rows - n / 2; ++i) {
            for(size_t j = m / 2; j < img.cols - m / 2; ++j) {
                result.at<cv::Vec3b>(i, j)[channel] = getPixel(img, channel, n, m, i, j);
            }
        }
    }

};