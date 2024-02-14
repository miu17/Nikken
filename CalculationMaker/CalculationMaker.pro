#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T11:52:53
#
#-------------------------------------------------

QT       += core gui opengl xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalculationMaker
TEMPLATE = lib

# for class export
DEFINES += CALCULATIONMAKER_LIB

P3DLIBROOT = $$PWD/../post3DLib
CONFIG( debug, debug|release){
        TARGET = $$join(TARGET,,,d)
        CALCULATIONMAKERMODE = debug
        P3DLIBNAME = post3DLibd
        GLVIEWERLIBNAME = QGLViewerd2
}
CONFIG( release, debug|release){
        CALCULATIONMAKERMODE = release
        P3DLIBNAME = post3DLib
        GLVIEWERLIBNAME = QGLViewerd
}

LIBS += -L$${P3DLIBROOT}/lib -l$${P3DLIBNAME}
LIBS += -L$$PWD/../libQGLViewer-2.9.1/QGLViewer -l$${GLVIEWERLIBNAME}

INCLUDEPATH += $${P3DLIBROOT}/include $$PWD/../libQGLViewer-2.9.1
DEPENDPATH += $${P3DLIBROOT}/lib

# Header files for install
headerFiles.path = $$PWD/include
headerFiles.files = \
           $$PWD/calculationmaker.h \
           $$PWD/calculationmakercommandinterface.h \
           $$PWD/drawinputfigure.h \
           $$PWD/drawoutputfigure.h \
           $$PWD/drawtable.h \
           $$PWD/figureinterface.h \
           $$PWD/tableinterface.h \

INSTALLS += headerFiles

# Library files for install
libraryFiles.path = $$PWD/lib
libraryFiles.files = $$OUT_PWD/$${CALCULATIONMAKERMODE}/$${TARGET}.lib $$OUT_PWD/$${CALCULATIONMAKERMODE}/$${TARGET}.dll
libraryFiles.CONFIG += no_check_exist
INSTALLS += libraryFiles



SOURCES += \
    calculationmaker.cpp \
    calculationmakerresourceparser.cpp \
    calculationmakercommandinterface.cpp \
    calculationmakerdrawcommands.cpp \
    calculationmakerconfigparser.cpp \
    calculationmakertextsnippet.cpp \
    calculationmakercontext.cpp \
    calculationmakerutils.cpp \
    drawinputfigure.cpp \
    drawoutputfigure.cpp \
    drawtable.cpp \
    figureinterface.cpp \
    tableinterface.cpp \


HEADERS  += \
    calculationmaker.h \
    calculationmakerresourceparser.h \
    calculationmakercommandinterface.h \
    calculationmakerdrawcommands.h \
    calculationmakerconfigparser.h \
    calculationmakersnippet.h \
    calculationmakercontext.h \
    calculationmakerutils.h \
    drawinputfigure.h \
    drawoutputfigure.h \
    drawtable.h \
    figureinterface.h \
    tableinterface.h \

#for clean
# 置換関数の宣言（使うところより前） windows対応
defineReplace(replaceCommand){
    DEPEND_FILE = $$1
    DEPEND_FILE ~= s|/|\|gi
    return($$DEPEND_FILE)
}

exists($$replaceCommand($$PWD/include)){
    QMAKE_PRE_LINK += del /Q  $$replaceCommand($$PWD/include/*.*)
}
