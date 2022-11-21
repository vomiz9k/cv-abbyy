#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "BaseFilter.h"
#include "Huang.h"
#include "Naive.h"
#include "Perreault.h"
#include "OpenCVFilter.h"

int main(int argc, char* argv[]) {
    const cv::Mat image = cv::imread(argv[1]);
    int n = std::atoi(argv[2]);
    int m = std::atoi(argv[3]);

    auto naive = NaiveFilter();
    auto huang = HuangFilter();
    auto perrault = PerreaultFilter();
    auto opencv = OpenCVFilter();

    std::vector<BaseFilter*> filters = {
        &naive,
        &huang,
        &perrault,
        &opencv,
    };

    int i = 0;
    for (auto filter_ptr: filters) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        auto result = filter_ptr->process(image, n, m);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        auto sec_per_megapixel = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / image.rows / image.cols;
        std::cout << sec_per_megapixel << ' ';
        cv::imwrite("images/" + std::to_string(n) + "result" + std::to_string(++i) + ".bmp", result);
    }
}