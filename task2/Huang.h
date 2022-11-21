#include <opencv2/opencv.hpp>
#include <iostream>
#include "BaseFilter.h"

class HuangFilter : public BaseFilter {
public:
    uchar getPixel(const cv::Mat& img, int channel, int n, int m, int x, int y) {
        for(size_t k = x - n / 2; k <= x + n / 2; ++k) {
            uchar to_delete = img.at<cv::Vec3b>(k, y - m / 2 - 1)[channel];
            uchar to_insert = img.at<cv::Vec3b>(k, y + m / 2)[channel];
            if (to_insert < median) {
                ++less_than_median;
            }
            if (to_delete < median) {
                --less_than_median;
            }
            ++hists[to_insert];
            --hists[to_delete];
            if (hists[to_delete] == 0) {
                hists.erase(to_delete);
            }
        }


        auto median_iter = hists.lower_bound(median);
        if (median_iter == hists.end()) { // m == 1
            median_iter = hists.begin();
            less_than_median = 0;
        }

        while (median_iter != hists.begin() && less_than_median > n * m / 2) {
            less_than_median -= (--median_iter)->second;
        }

        while (less_than_median + median_iter->second <= n * m / 2) {
            less_than_median += (median_iter++)->second;;
        }

        median = median_iter->first;
        return median;
    }

    uchar initialize(const cv::Mat& img, int row, int n, int m, int channel) {
        hists.clear();
        for(size_t i = row - n / 2; i <= row + n / 2; ++i) {
            for(size_t j = 0; j < m; ++j) {
                ++hists[img.at<cv::Vec3b>(i, j)[channel]];
            }
        }
        auto iter = hists.begin();
        for (less_than_median = 0; less_than_median + iter->second <= n * m / 2; ++iter) {
            less_than_median += iter->second;
        }
        median = iter->first;
        return median;
    }

    virtual void processChannel(const cv::Mat& img, cv::Mat& result, int n, int m, int channel) override {

        for(size_t i = n / 2; i < img.rows - n / 2; ++i) {
            result.at<cv::Vec3b>(i, m / 2)[channel] = initialize(img, i, n, m, channel);
            for(size_t j = m / 2 + 1; j < img.cols - m / 2; ++j) {
                result.at<cv::Vec3b>(i, j)[channel] = getPixel(img, channel, n, m, i, j);
            }
        }
    }

    std::map<uchar, size_t> hists;
    uchar median;
    int less_than_median;
};
