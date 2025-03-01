#-------------------------------------------------
#
# Project created by QtCreator 2022-04-06T15:53:51
#
#-------------------------------------------------

QT       += core gui
#添加第三方的库时配置路经
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui

TARGET = VideoCaptureQt
TEMPLATE = app
RESOURCES += \
    resources.qrc
#文件分类
SOURCES += main.cpp\
        Lstr.cpp \
        mainwindow.cpp \
        yolov7.cpp \
        speechsynthesis.cpp

HEADERS  += mainwindow.h \
    Lstr.h \
    yolov7.h\
    speechsynthesis.h

FORMS    += mainwindow.ui



INCLUDEPATH +=/home/acoinfo/VideoCaptureQt/include
DESTDIR=$$/home/acoinfo/VideoCaptureQt/bin
LIBS +=/home/acoinfo/VideoCaptureQt/libs/x64/libmsc.so

#添加第三方的库时配置路经
unix:!macx: LIBS += /usr/local/lib/libopencv_* \
                    /usr/local/lib/libonnxruntime* \


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \
                /home/acoinfo/onnxruntime-linux-x64-1.12.0/include \
DEPENDPATH += $$PWD/x64/lib

