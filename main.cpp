#include "filters.hpp"
#include <iostream>
// #include <opencv2/opencv.hpp>

#include <vector>

int main(int, char **) {

    cv::Mat image;

    image = cv::imread("forest_image.jpg", 1);
    if (!image.data) {
        std::cout << "no image data \n";
        return 1;
    }
    cv::namedWindow("Forest Image No Blur", cv::WINDOW_AUTOSIZE);
    cv::imshow("Forest Image No Blur", image);
    gaussian_blur_filter blur(1.5, image, 5);

    cv::namedWindow("Forest Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Forest Image", blur.image);
    cv::waitKey(0);
    return 0;
}

void change_img(cv::Mat &image) {
    CV_Assert(image.depth() == CV_8U);

    int channels = image.channels();

    int n_rows = image.rows;
    int n_cols = image.cols * channels;

    if (image.isContinuous()) {
        n_cols *= n_rows;
        n_rows = 1;
    }

    uchar *p;
    for (int i = 0; i < n_rows; ++i) {
        p = image.ptr<uchar>(i);

        for (int j = 0; j < n_cols - 3; j += 3) {
            uchar blue = p[j];
            uchar green = p[j + 1];
            uchar red = p[j + 2];
            p[j] = red;
            p[j + 1] = blue;
            p[j + 2] = green;
        }
    }
}

void encode_into_img(cv::Mat &original_image, const std::string &message_to_encode) {
    uchar *p_blue = original_image.ptr<uchar>(0);

    for (size_t i = 0; i < message_to_encode.size(); ++i) {
        p_blue[i] = message_to_encode[i];
    }
}

std::string decode_img(const cv::Mat &original_image, size_t image_size) {
    std::string output;
    const uchar *p_blue = original_image.ptr<uchar>(0);

    for (size_t i = 0; i < image_size; ++i) {
        output += p_blue[i];
    }

    return output;
}
