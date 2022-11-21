#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
class BaseFilter {

    virtual void processChannel(const cv::Mat& img, cv::Mat& result, int n, int m, int channel) {};

public:
    virtual cv::Mat process(const cv::Mat& img, int n, int m) {
        cv::Mat result;
        cv::Mat padded_image;
        copyMakeBorder(img, padded_image, n / 2, n / 2, m / 2, m / 2, cv::BORDER_REPLICATE);
        padded_image.copyTo(result);
        for (int channel = 0; channel < img.channels(); ++channel) {
            processChannel(padded_image, result, n, m, channel);
        }
        return result(cv::Range(n / 2, img.rows + n / 2), cv::Range(m / 2, img.cols + m / 2));
    }

    virtual ~BaseFilter() = default;

};