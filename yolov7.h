#ifndef YOLOV7_H
#define YOLOV7_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//#include <cuda_provider_factory.h>
#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace cv;
using namespace Ort;

struct Net_config
{
    float confThreshold; // Confidence threshold
    float nmsThreshold;  // Non-maximum suppression threshold
    string modelpath;
};

typedef struct BoxInfo
{
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

class YOLOV7
{
public:
    YOLOV7(Net_config config);
    //YOLOV7(const Net_config& config);
    void detect(Mat& frame);
    bool isPersonDetected() const { return personDetected; }
    bool islightDetected() const { return lightDetected; }
    bool issignDetectedD() const { return signDetected; }
    bool isvehicleDetected() const { return vehicleDetected; }

private:
    bool personDetected; // 标记是否检测到 person 目标
    bool lightDetected; // 标记是否检测到 light 目标(红绿灯)
    bool signDetected; // 标记是否检测到 sign 目标(停车标识)
    bool vehicleDetected;   // 表示是否检测到车辆


    int inpWidth;
    int inpHeight;
    int nout;
    int num_proposal;
    vector<string> class_names;
    int num_class;

    float confThreshold;
    float nmsThreshold;
    vector<float> input_image_;
    void normalize_(Mat img);
    void nms(vector<BoxInfo>& input_boxes);

    Env env = Env(ORT_LOGGING_LEVEL_ERROR, "YOLOV7");
    Ort::Session *ort_session = nullptr;
    SessionOptions sessionOptions = SessionOptions();
    vector<char*> input_names;
    vector<char*> output_names;
    vector<vector<int64_t>> input_node_dims; // >=1 outputs
    vector<vector<int64_t>> output_node_dims; // >=1 outputs
};
#endif // YOLOV7_H
