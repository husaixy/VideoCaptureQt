#include "Lstr.h"
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <onnxruntime_cxx_api.h>

using namespace cv;
using namespace std;
using namespace Ort;

LSTR::LSTR()
{
    // 构造函数的实现
}

LSTR::~LSTR()
{
    // 析构函数的实现
}

void LSTR::normalize_(Mat img)
{
    // normalize_ 函数的实现
}

cv::Mat LSTR::detect(cv::Mat& srcimg)
{
    // detect 函数的实现
}
