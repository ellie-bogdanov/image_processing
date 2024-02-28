#pragma once
#include <iterator>
#include <opencv2/opencv.hpp>

struct image_filter {
    cv::Mat image;

    image_filter(cv::Mat &image);
    ~image_filter();

    virtual void apply_filter() = 0;
};

struct gaussian_blur_filter : public image_filter {
    std::vector<std::vector<double>> kernel;
    size_t radius;
    double sigma;

    gaussian_blur_filter(double sigma, cv::Mat &image, size_t weight_matrix_size);
    ~gaussian_blur_filter();

    double gaussian_blur_function(double x, double y);

    void create_kernel();

    void apply_filter();
};