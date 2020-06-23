#-------------------------------------------------
#
# Project created by QtCreator 2019-04-08T17:30:53
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D_Human_Landmarks_Labeling
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    viewer.cpp

HEADERS += \
        mainwindow.h \
    viewer.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux{
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/include
#    LIBS += -lCGAL -lCGAL_Core -lgmp  -lmpfr
    LIBS += -lQGLViewer-qt5 -lGL -lGLU -lglut
}



INCLUDEPATH += D:\Downloads\Libs_for_Windows\eigen3_3_4\eigen \
INCLUDEPATH += D:\Downloads\Libs_for_Windows\libigl\include \
INCLUDEPATH += D:\Downloads\Libs_for_Windows\libQGLViewer_2_7_1\include \
INCLUDEPATH += D:\Downloads\Libs_for_Windows\freeglut\include
#LIBS += -lopengl32
LIBS += -LD:\Downloads\Libs_for_Windows\freeglut\vs2017_x64\lib\Debug -lfreeglutd
LIBS += -LD:\Downloads\Libs_for_Windows\libQGLViewer_2_7_1\libs -lQGLViewerd2
LIBS += -L$$quote(C:\Program Files (x86)\Windows Kits\10\Lib\10.0.16299.0\um\x64) -lOpenGL32

