#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "yolov7.h"
#include <QProcess>
#include <QTimer>
#include <QThread>
#include <speechsynthesis.h>

//命名空间 UI界面中的MainWindow和下面的不是同一个
namespace Ui {
class MainWindow;
}
class ClientProgramThread : public QThread
{
public:
    QString executablePath;
    QStringList arguments;
    void run() override
    {
        QProcess process;
        process.start(executablePath, arguments);
        process.waitForFinished();
        process.close();
    }
};
class DetectionThread : public QThread
{
public:
    DetectionThread(const QString& text) : synthesisText(text) {}

    void run() override
    {


        // 语音合成代码
        SpeechSynthesis tts;
        const char* session_begin_params = "engine_type = local,voice_name=xiaoyan, text_encoding = UTF8, tts_res_path = fo|res/tts/xiaoyan.jet;fo|res/tts/common.jet, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
        const char* filename = "tts_sample.wav";

        const char* text = synthesisText.toUtf8().constData(); // 使用传递的文本
        std::string textString(text); // 转换为std::string类型的字符串
        qDebug("停车标识检测：%s", textString.c_str()); // 输出合成的文本
        int ret = tts.textToSpeech(textString.c_str(), filename, session_begin_params);
        if (ret == 0) {
            qDebug("合成成功");
            // 在这里播放合成的语音
            std::string command = "aplay " + std::string(filename);
            system(command.c_str());
        } else {
            qDebug("合成失败，错误码：%d", ret);
        }

        tts.~SpeechSynthesis();
    }

private:
    QString synthesisText;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int flag=0;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//Qt框架中的一个扩展，用于定义槽函数。
public slots:
    void updateImage();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void on_btnBrowseCam_clicked();

    void on_btnOpenCam_clicked();

    void on_btnCloseCam_clicked();

    //void on_btnStartCapture_clicked();

private:
    Ui::MainWindow *ui;
//一个用于捕获视频流的OpenCV类对象。它可以用于从摄像头或视频文件中读取图像序列。
    cv::VideoCapture capture;
//    定时器类对象。它可以用于定期执行一个特定的槽函数或操作。
    QTimer theTimer;
//    cv::Mat srcImage;
    cv::Mat srcImage;
//    image_fliped 是OpenCV中的图像矩阵，用于存储图像数据
    cv::Mat image_fliped;
    int index = 0;
    QString filePath;
    YOLOV7 net;
    void executeClientProgram();
    ClientProgramThread* clientThread = nullptr;
    ClientProgramThread* clientThread2 = nullptr;
    ClientProgramThread* clientThread3 = nullptr;
    ClientProgramThread* clientThread4 = nullptr;
    ClientProgramThread* clientThread5 = nullptr;
    ClientProgramThread* clientThread6 = nullptr;
    ClientProgramThread* clientThread7 = nullptr;

    DetectionThread* detectionThread = nullptr;
};

#endif // MAINWINDOW_H
