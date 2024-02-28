#include "filters.hpp"
#include <cmath>

image_filter::image_filter(cv::Mat &image) : image(image) {
}

image_filter::~image_filter() {
}

gaussian_blur_filter::gaussian_blur_filter(double sigma, cv::Mat &image, size_t weight_matrix_size) : sigma(sigma), image_filter(image), radius(weight_matrix_size) {

    // std::vector<double> col(image.cols, 1);

    create_kernel();
    apply_filter();
}

double gaussian_blur_filter::gaussian_blur_function(double x, double y) {
    double exponent = -(std::pow(x, 2) + std::pow(y, 2)) / (2 * std::pow(sigma, 2));
    return std::exp(exponent) / (2 * M_PI * std::pow(sigma, 2));
}

void gaussian_blur_filter::create_kernel() {

    int mid_point = (radius / 2);
    double sum = 0;

    for (double y = -mid_point; y <= mid_point; ++y) {
        std::vector<double> kernel_col;
        for (double x = -mid_point; x <= mid_point; ++x) {

            double blur_mult = gaussian_blur_function(y, x);
            sum += blur_mult;
            kernel_col.push_back(blur_mult);
        }
        kernel.push_back(kernel_col);
    }
    for (size_t j = 0; j < kernel.size(); ++j) {
        for (size_t i = 0; i < kernel.size(); ++i) {
            kernel[j][i] / sum;
        }
    }
}

void gaussian_blur_filter::apply_filter() {
    if (image.channels() == 3) {
        cv::Mat_<cv::Vec3b> _image = image;
        for (size_t y = 0; y < image.rows - kernel.size(); ++y) {

            for (size_t x = 0; x < image.cols - kernel.size(); ++x) {
                double blue = 0.0;
                double green = 0.0;
                double red = 0.0;
                for (size_t j = 0; j < kernel.size(); ++j) {
                    for (size_t i = 0; i < kernel.size(); ++i) {
                        blue += _image(y + j, x + i)[0] * kernel[j][i];
                        green += _image(y + j, x + i)[1] * kernel[j][i];
                        red += _image(y + j, x + i)[2] * kernel[j][i];
                    }
                }
                _image(y, x)[0] = blue;
                _image(y, x)[1] = green;
                _image(y, x)[2] = red;
            }
        }
        image = _image;
    } else if (image.channels() == 1) {
        size_t n_rows = image.rows;
        size_t n_cols = image.cols;
        uchar *p;
        for (size_t y = 0; y < n_rows - kernel.size(); ++y) {

            for (size_t x = 0; x < n_cols - kernel.size(); ++x) {

                double gray = 0.0;

                for (size_t j = 0; j < kernel.size(); ++j) {
                    p = image.ptr<uchar>(y + j);
                    for (size_t i = 0; i < kernel.size(); ++i) {
                        gray += p[x + i] * kernel[j][i];
                    }
                }
                p[x] = gray;
            }
        }
    }
}

gaussian_blur_filter::~gaussian_blur_filter() {}