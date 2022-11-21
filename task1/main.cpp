#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

class PixelGrouping {
private:
    enum Colors {
        Blue = 0,
        Green = 1,
        Red = 2
    };

    static int getColor(int x, int y) {
        if (x % 2 == 0 && y % 2 == 0) {
            return Colors::Red;
        }
        if (x % 2 != 0 && y % 2 != 0) {
            return Colors::Blue;
        }
        return Colors::Green;
    }

    static int hueTransit(int L1, int L2, int L3, int V1, int V3) {
        if ((L1 < L2 && L2 < L3) || (L3 < L2 && L2 < L1)) {
            return V1 + (V3 - V1) * (L2 - L1) / (L3 - L1);
        }
        return (V1 + V3) / 2 + (L2 - (L1 + L3) / 2) / 2;
    }

    static unsigned char filterValue(int value) {
        if (value < 0) {
            return 0;
        }
        if (value > 255) {
            return 255;
        }
        return value;
    }

    #define A1 img.at<cv::Vec3b>(i - 2, j - 2)[getColor(i - 2, j - 2)]
    #define B1 img.at<cv::Vec3b>(i - 2, j - 1)[getColor(i - 2, j - 1)]
    #define C1 img.at<cv::Vec3b>(i - 2, j)[getColor(i - 2, j)]
    #define D1 img.at<cv::Vec3b>(i - 2, j + 1)[getColor(i - 2, j + 1)]
    #define E1 img.at<cv::Vec3b>(i - 2, j + 2)[getColor(i - 2, j + 2)]

    #define A2 img.at<cv::Vec3b>(i - 1, j - 2)[getColor(i - 1, j - 2)]
    #define B2 img.at<cv::Vec3b>(i - 1, j - 1)[getColor(i - 1, j - 1)]
    #define C2 img.at<cv::Vec3b>(i - 1, j)[getColor(i - 1, j)]
    #define D2 img.at<cv::Vec3b>(i - 1, j + 1)[getColor(i - 1, j + 1)]
    #define E2 img.at<cv::Vec3b>(i - 1, j + 2)[getColor(i - 1, j + 2)]

    #define A3 img.at<cv::Vec3b>(i, j - 2)[getColor(i, j - 2)]
    #define B3 img.at<cv::Vec3b>(i, j - 1)[getColor(i, j - 1)]
    #define C3 img.at<cv::Vec3b>(i, j)[getColor(i, j)]
    #define D3 img.at<cv::Vec3b>(i, j + 1)[getColor(i, j + 1)]
    #define E3 img.at<cv::Vec3b>(i, j + 2)[getColor(i, j + 2)]

    #define A4 img.at<cv::Vec3b>(i + 1, j - 2)[getColor(i + 1, j - 2)]
    #define B4 img.at<cv::Vec3b>(i + 1, j - 1)[getColor(i + 1, j - 1)]
    #define C4 img.at<cv::Vec3b>(i + 1, j)[getColor(i + 1, j)]
    #define D4 img.at<cv::Vec3b>(i + 1, j + 1)[getColor(i + 1, j + 1)]
    #define E4 img.at<cv::Vec3b>(i + 1, j + 2)[getColor(i + 1, j + 2)]

    #define A5 img.at<cv::Vec3b>(i + 2, j - 2)[getColor(i + 2, j - 2)]
    #define B5 img.at<cv::Vec3b>(i + 2, j - 1)[getColor(i + 2, j - 1)]
    #define C5 img.at<cv::Vec3b>(i + 2, j)[getColor(i + 2, j)]
    #define D5 img.at<cv::Vec3b>(i + 2, j + 1)[getColor(i + 2, j + 1)]
    #define E5 img.at<cv::Vec3b>(i + 2, j + 2)[getColor(i + 2, j + 2)]

    #define Get(i, j, color) img.at<cv::Vec3b>(i, j)[color]

     /*
     A B C D E
    1
    2
    3
    4
    5
    это не особо пригодилось, думал сильно жизнь мне упростит
    */
public:
    static cv::Mat process(cv::Mat& img) {
        for (int i = 2; i < img.rows - 2; ++i) {
            for (int j = i % 2 ? 3 : 2; j < img.cols - 2; j += 2) {
                int delta_n = std::abs(C3 - C1) * 2 + std::abs(C4 - C2);
                int delta_e = std::abs(C3 - E3) * 2 + std::abs(B3 - D3);
                int delta_w = std::abs(C3 - A3) * 2 + std::abs(B3 - D3);
                int delta_s = std::abs(C3 - C5) * 2 + std::abs(C4 - C2);

                int delta_min = std::min({delta_n, delta_e, delta_w, delta_s});
                int G = 0;
                if (delta_n == delta_min) {
                    G = C2 * 3 + C4 + C3 - C1;
                } else if (delta_e == delta_min) {
                    G = D3 * 3 + B3 + C3 - E3;
                } else if (delta_w == delta_min) {
                    G = B3 * 3 + D3 + C3 - A3;
                } else {
                    G = C4 * 3 + C2 + C3 - C5;
                }
                G /= 4;

                img.at<cv::Vec3b>(i, j)[Colors::Green] = filterValue(G);
            }
        }
        for (int i = 2; i < img.rows - 2; ++i) {
            for(int j = i % 2 ? 2 : 1; j < img.cols - 1; j += 2) {
                img.at<cv::Vec3b>(i, j)[getColor(i, j + 1)] = filterValue(hueTransit(
                    Get(i, j - 1, Colors::Green),
                    C3,
                    Get(i, j + 1, Colors::Green),
                    B3,
                    D3
                ));
                img.at<cv::Vec3b>(i, j)[getColor(i + 1, j)] = filterValue(hueTransit(
                    Get(i - 1, j, Colors::Green),
                    C3,
                    Get(i + 1, j, Colors::Green),
                    C2,
                    C4
                ));
            }
        }

        for (int i = 2; i < img.rows - 2; ++i) {
            for (int j = i % 2 ? 3 : 2; j < img.cols - 2; j += 2) {
                int delta_ne = std::abs(D2 - B4) +
                               std::abs(E1 - C3) +
                               std::abs(C3 - A5) +
                               std::abs(Get(i - 1, j + 1, Colors::Green) - Get(i, j, Colors::Green)) +
                               std::abs(Get(i, j, Colors::Green) - Get(i + 1, j - 1, Colors::Green));

                int delta_nw = std::abs(B2 - D4) +
                               std::abs(A1 - C3) +
                               std::abs(C3 - E5) +
                               std::abs(Get(i - 1, j - 1, Colors::Green) - Get(i, j, Colors::Green)) +
                               std::abs(Get(i, j, Colors::Green) - Get(i + 1, j + 1, Colors::Green));

                auto color = getColor(i, j) == Colors::Blue ? Colors::Red : Colors::Blue;

                if (delta_ne < delta_nw) {
                    img.at<cv::Vec3b>(i, j)[color] = filterValue(hueTransit(
                        Get(i - 1, j + 1, Colors::Green),
                        Get(i, j, Colors::Green),
                        Get(i + 1, j - i, Colors::Green),
                        D2,
                        B4
                    ));
                } else {
                    img.at<cv::Vec3b>(i, j)[color] = filterValue(hueTransit(
                        Get(i - 1, j - 1, Colors::Green),
                        Get(i, j, Colors::Green),
                        Get(i + 1, j + 1, Colors::Green),
                        B2,
                        D4
                    ));
                }
            }
        }
        return img;
    }

    #undef everything
};

#include <iostream>

int main(int argc, char* argv[]) {
    cv::Mat image = cv::imread(argv[1]);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto result = PixelGrouping::process(image);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto sec_per_megapixel = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / image.rows / image.cols;
    std::cout << sec_per_megapixel << std::endl;
    cv::imwrite("result.bmp", result);
}