#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//#include <cuda_provider_factory.h>
#include <onnxruntime_cxx_api.h>
#include <mainwindow.h>
#include <QProcess>
#include <QTimer>
#include <QThread>
#include <yolov7.h>
#include <speechsynthesis.h>
#include <cstdlib>
#include <csignal>
using namespace std;
using namespace cv;
Net_config YOLOV7_nets = { 0.6, 0.8, "/home/acoinfo/untitled/weights/yolov6n.onnx" };


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
//    实例化MainWindow(UI)
    ui(new Ui::MainWindow),
    net(YOLOV7_nets)
{

//    绑定两个MainWindow
    ui->setupUi(this);
    //样式设置
        ui->lblStatus->setStyleSheet("QLabel { border: 1px solid black; background-color: white; }");//设置样式表
        ui->imageLabel->setStyleSheet("QLabel { border: 3px solid black; background-color: blue}");
        ui->sideBar->setStyleSheet("background-color: rgb(228, 228, 228);border-right: 3px solid black;");
    //    建立信号和槽的连接。当 theTimer 定时器的时间到达后，会发出 timeout() 信号，然后该信号与 updateImage() 槽函数建立连接
        connect(&theTimer, SIGNAL(timeout()), this, SLOT(updateImage()));

        on_btnOpenCam_clicked();
}
//    析构函数
MainWindow::~MainWindow()
{
    if(capture.isOpened())
        capture.release();
    delete ui;
}

void MainWindow::on_btnBrowseCam_clicked()
{
    if (capture.isOpened())
        return;
    //    用户选择视频文件
        QString filePath2 = QFileDialog::getOpenFileName(this, "选择视频文件", "/home/acoinfo/VideoCaptureQt", "视频文件 (*.mov *.mp4)");
        filePath=filePath2;
        on_btnOpenCam_clicked();
}

void MainWindow::on_btnOpenCam_clicked()
{

    DetectionThread* detectionThread = new DetectionThread("另");
    // 创建新线程并执行executeClientProgram
    ClientProgramThread* clientThread = new ClientProgramThread;
    clientThread->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
    clientThread->arguments << "speed" << "100";
    clientThread->start();
    // 创建第二个 ClientProgramThread 对象，设置参数 "brakingwaring" 和 "0"
    ClientProgramThread* clientThread2 = new ClientProgramThread;
    clientThread2->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
    clientThread2->arguments << "brakingwaring" << "0";
    clientThread2->start();
    // 创建第三个 ClientProgramThread 对象，设置参数 "gear" 和 "3"
    ClientProgramThread* clientThread3 = new ClientProgramThread;
    clientThread3->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
    clientThread3->arguments << "gear" << "3";
    clientThread3->start();
    if (capture.isOpened())
        return;
    std::string file="/home/acoinfo/VideoCaptureQt/video/demo/1xingren.mp4";//视频地址
    if (!filePath.isEmpty()) {
        file = filePath.toStdString();
    }
    ui->lblStatus->setText(QString::fromStdString(file));
    if (capture.open(file)) {
        cout<<"cam is  opened"<<endl;
        //创建一个大小与视频帧高度和宽度相同、像素格式为 8 位无符号整数的空白图像 srcImage
        srcImage = Mat::zeros(capture.get(CAP_PROP_FRAME_HEIGHT), capture.get(CAP_PROP_FRAME_WIDTH), CV_8UC3);
//        启动一个定时器 theTimer，设置其超时时间为 33 毫秒（约为每秒 30 帧）
        theTimer.start(33);
    }else {
        cout<<"video file open failed."<<endl;
        capture.release();

    }
}


void MainWindow::on_btnCloseCam_clicked()
{
    capture.release();
    detectionThread = nullptr;
}

//    该函数用于在界面上绘制图像，并更新名为 imageLabel 的标签显示最新的图像。
void MainWindow::paintEvent(QPaintEvent *e)
{
//    一个包含图像像素值的矩阵）来构造图像，并指定图像的宽度、高度和像素格式为 RGB888。
    QImage image1 = QImage((uchar*)(image_fliped.data), image_fliped.cols, image_fliped.rows, QImage::Format_RGB888);
//    图像就被设置为 imageLabel 标签的内容
    ui->imageLabel->setPixmap(QPixmap::fromImage(image1));
    ui->imageLabel->resize(image1.size());
    ui->imageLabel->show();
}
//    实现实时图像处理和展示
//Net_config YOLOV7_nets = { 0.6, 0.6, "/home/acoinfo/untitled/weights/yolov7-tiny_640x640.onnx" };
//YOLOV7 net(YOLOV7_nets);
void MainWindow::updateImage()
{

    const char* text = "坎坷坷语音识别、口语评测等多项技术上拥有技术成果。";
    capture >> srcImage;
    image_fliped = srcImage.clone();
    if (image_fliped.data)
    {
        cvtColor(image_fliped, image_fliped, COLOR_BGR2RGB);
        net.detect(image_fliped);
        // 行人检测
        if (net.isPersonDetected())
        {
            qDebug("行人检测");
            //这里语音提醒，创建并启动线程
            text = "检测到行人，请减速慢行";
                if (!detectionThread)
                       {
                     detectionThread =new DetectionThread(text);
                     detectionThread->start();
                       }

            if (!clientThread)
            {
                clientThread = new ClientProgramThread;
                clientThread->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
                clientThread->arguments << "speed" << "1";
                clientThread->start();
            }

            if (!clientThread2)
            {
                clientThread2 = new ClientProgramThread;
                clientThread2->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
                clientThread2->arguments << "brakingwaring" << "1";
                clientThread2->start();
            }


        }
        // 红绿灯检测
        if (net.islightDetected())
        {
            qDebug("红绿灯检测");
            text = "检测到当前处于红灯，请立即停车";
            if (!detectionThread)
                   {
                 detectionThread =new DetectionThread(text);
                 detectionThread->start();
                   }
            if (!clientThread3)
            {
                clientThread3 = new ClientProgramThread;
                clientThread3->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
                clientThread3->arguments << "speed" << "1";
                clientThread3->start();
            }

            if (!clientThread4)
            {
                clientThread4 = new ClientProgramThread;
                clientThread4->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
                clientThread4->arguments << "gear" << "2";
                clientThread4->start();
            }
        }

        //停车标识检测
        if (net.issignDetectedD())
        {
            qDebug("停车标识检测");
            text = "检测到停车牌，请立即停车";
            if (!detectionThread)
                   {
                 detectionThread =new DetectionThread(text);
                 detectionThread->start();
                   }
            if (!clientThread5)
            {
                clientThread5 = new ClientProgramThread;
                clientThread5->executablePath = "/home/acoinfo/vsoa_c_demo/linux64-build/client";
                clientThread5->arguments << "speed" << "80";
                clientThread5->start();
            }

        }
        this->update();
    }
}

