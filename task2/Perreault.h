#include <opencv2/opencv.hpp>
#include <iostream>
#include "BaseFilter.h"


class PerreaultFilter : public BaseFilter {
    uchar getPixel(const cv::Mat& img, int channel, int n, int m, int x, int y) {
        if (x - n / 2 - 1 >= 0) {
            --row_hists[y + m / 2][img.at<cv::Vec3b>(x - n / 2 - 1, y + m / 2)[channel]];
            ++row_hists[y + m / 2][img.at<cv::Vec3b>(x + n / 2, y + m / 2)[channel]];
        }

        for(size_t i = 0; i < median; ++i) {
            int delta = row_hists[y + m / 2][i] - row_hists[y - m / 2 - 1][i];
            window_hists[i] += delta;
            less_than_median += delta;
        }
        for (size_t i = median; i < 256; ++i) {
            int delta = row_hists[y + m / 2][i] - row_hists[y - m / 2 - 1][i];
            window_hists[i] += delta;
        }

        while (less_than_median > n * m / 2) {
            less_than_median -= window_hists[--median];
        }
        while (less_than_median + window_hists[median] <= n * m / 2) {
            less_than_median += window_hists[median++];
        }

        return median;
    }


    void initializeRowHists(const cv::Mat& img, int n, int channel) {
        row_hists.resize(img.cols);
        for(int j = 0; j < img.cols; ++j) {
            std::fill(row_hists[j].begin(), row_hists[j].end(), 0);
            for (int i = 0; i < n; ++i) {
                ++row_hists[j][img.at<cv::Vec3b>(i, j)[channel]];
            }
        }
    }

    uchar initialize(const cv::Mat& img, int row, int n, int m, int channel) {
        std::fill(window_hists.begin(), window_hists.end(), 0);

        for(size_t i = row - n / 2; i <= row + n / 2; ++i) {
            for(size_t j = 0; j < m; ++j) {
                uchar curr = img.at<cv::Vec3b>(i, j)[channel];
                ++window_hists[curr];
            }
        }

        if (row - n / 2 - 1 >= 0) {
            for(size_t j = 0; j < m; ++j) {
                ++row_hists[j][img.at<cv::Vec3b>(row + n / 2, j)[channel]];
                --row_hists[j][img.at<cv::Vec3b>(row - n / 2 - 1, j)[channel]];
            }
        }

        median = 0;
        less_than_median = 0;
        while(less_than_median + window_hists[median] <= n * m / 2) {
            less_than_median += window_hists[median++];
        }
        return median;
    }

    virtual void processChannel(const cv::Mat& img, cv::Mat& result, int n, int m, int channel) override {
        initializeRowHists(img, n, channel);
        for(size_t i = n / 2; i < img.rows - n / 2; ++i) {
            result.at<cv::Vec3b>(i, m / 2)[channel] = initialize(img, i, n, m, channel);
            for(size_t j = m / 2 + 1; j < img.cols - m / 2; ++j) {
                result.at<cv::Vec3b>(i, j)[channel] = getPixel(img, channel, n, m, i, j);
            }
        }
    }


    std::array<int, 256> window_hists;
    std::vector<std::array<int, 256>> row_hists;
    uchar median = 0;
    int less_than_median = 0;
};
