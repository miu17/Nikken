TEMPLATE = app

QT += testlib opengl svg widgets printsupport xml
QT -= gui

DEPENDPATH += ..

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++11

DEFINES += POST3D_LIB

#以下、intelコンパイラのifconsol.libに依存
LIBS += -l$$PWD/../../NewBUILDING3D/3dcal/lib/3dcal_comm
LIBS += -l$$PWD/../../NewBUILDING3D/3dcal/lib/3dcal_pos1
LIBS += -l$$PWD/../../NewBUILDING3D/3dcal/lib/3dcal_etc
LIBS += -l$$PWD/../../NewBUILDING3D/3dcal/lib/3dcal_qdcv
LIBS += -L"$$PWD/../../fortranLib/IntelFortran/2021.4.0"


LIBS += -lOpenGL32
LIBS *= -L$$PWD/../../libQGLViewer-2.6.4/QGLViewer
LIBS *= -lQGLViewer2

INCLUDEPATH *= $$PWD/../../libQGLViewer-2.6.4

SOURCES +=  \
    main.cpp


include(../post3DLib.pri)
