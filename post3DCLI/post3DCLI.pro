#QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
QT *= opengl xml svg widgets printsupport svg

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# parent direcotry of "include" and "lib" for post3dLib.
P3DLIBROOT = $$PWD/../post3DLib
CALCULATIONMAKERLIBROOT = $$PWD/../CalculationMaker
CONFIG( debug, debug|release ):{
    P3DLIBNAME = post3DLibd
    GLVIEWERLIBNAME = QGLViewerd2
    CALCULATIONMAKERLIBNAME = CalculationMakerd
    BUILD_TYPE = debug
}
CONFIG( release, debug|release ):{
    P3DLIBNAME = post3DLib
    GLVIEWERLIBNAME = QGLViewer2
    CALCULATIONMAKERLIBNAME = CalculationMaker
    BUILD_TYPE = release
}

LIBS += -lOpenGL32
LIBS += -L$${P3DLIBROOT}/lib -l$${P3DLIBNAME}
LIBS += -L$$PWD/../libQGLViewer-2.9.1/QGLViewer -l$${GLVIEWERLIBNAME}
LIBS += -L$${CALCULATIONMAKERLIBROOT}/lib -l$${CALCULATIONMAKERLIBNAME}
INCLUDEPATH += . $${P3DLIBROOT}/include
INCLUDEPATH += $$PWD/../libQGLViewer-2.9.1
INCLUDEPATH += $${CALCULATIONMAKERLIBROOT}/include
DEPENDPATH += . $${P3DLIBROOT}/lib
DEPENDPATH += $${CALCULATIONMAKERLIBROOT}/lib

SOURCES += \
        cui_runner.cpp \
        main.cpp \
        run_figure.cpp \
        run_input.cpp \
        run_statement.cpp \
        run_text.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cui_runner.h \
    run_figure.h \
    run_input.h \
    run_statement.h \
    run_text.h
