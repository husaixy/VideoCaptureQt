#ifndef LSTR_H
#define LSTR_H

#include <opencv2/core/core.hpp>

class LSTR
{
public:
    LSTR();
    cv::Mat detect(cv::Mat& cv_image);
    ~LSTR();  // 析构函数, 释放内存

private:
    void normalize_(cv::Mat img);

private:
    int inpWidth;
    int inpHeight;
    std::vector<float> input_image_;
    std::vector<float> mask_tensor;
    float mean[3] = { 0.485, 0.456, 0.406 };
    float std[3] = { 0.229, 0.224, 0.225 };
    const int len_log_space = 50;
    float* log_space;
    cv::Scalar lane_colors[8] = { cv::Scalar(68,65,249), cv::Scalar(44,114,243), cv::Scalar(30,150,248),
                                  cv::Scalar(74,132,249), cv::Scalar(79,199,249), cv::Scalar(109,190,144),
                                  cv::Scalar(142, 144, 77), cv::Scalar(161, 125, 39) };

//    Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_ERROR, "LSTR");
//    Ort::Session* ort_session = nullptr;
//    Ort::SessionOptions sessionOptions = Ort::SessionOptions();
    std::vector<const char*> input_names;
    std::vector<const char*> output_names;
    std::vector<std::vector<int64_t>> input_node_dims; // >=1 outputs
    std::vector<std::vector<int64_t>> output_node_dims; // >=1 outputs;
};

#endif // LSTR_H
