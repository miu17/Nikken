TEMPLATE = app
TARGET = post3DMain
DISTFILES += $$PWD/../beautifier/p3d.astylerc
TRANSLATIONS = $$PWD/app/translate/post3D_ja.ts

# https://developex.com/blog/how-to-make-your-c-qt-project-build-10x-faster-with-4-optimizations/
QMAKE_CXXFLAGS += -MP
PRECOMPILED_HEADER = $$PWD/precompile.h
CONFIG += precompile_header


# parent direcotry of "include" and "lib" for post3dLib.
P3DLIBROOT = $$PWD/../post3DLib
CALCULATIONMAKERLIBROOT = $$PWD/../CalculationMaker
CONFIG( debug, debug|release ):{
    P3DLIBNAME = post3DLibd
    GLVIEWERLIBNAME = QGLViewerd2
    CALCULATIONMAKERLIBNAME = CalculationMakerd
    BUILD_TYPE = debug
    PRINTSUPPORTDLLNAME = Qt5PrintSupportd
    XMLDLLNAME = Qt5Xmld
}
CONFIG( release, debug|release ):{
    P3DLIBNAME = post3DLib
    GLVIEWERLIBNAME = QGLViewer2
    CALCULATIONMAKERLIBNAME = CalculationMaker
    BUILD_TYPE = release
    PRINTSUPPORTDLLNAME = Qt5PrintSupport
    XMLDLLNAME = Qt5Xml
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

CONFIG *= qt opengl　c++11
QT *= opengl xml widgets printsupport svg


include (./app/app.pri)
include (./calculation/calculation.pri)
include (./calculation_manager/calculation_manager.pri)
include (./common/common.pri)
include (./central_area/central_area.pri)
include (./data_unified/data_unified.pri)
include (./custom_table/custom_table.pri)
include (./custom_drawing/custom_drawing.pri)
include (./interface_plan/interface_plan.pri)
include (./interface_elevation/interface_elevation.pri)
include (./interface_graph/interface_graph.pri)
include (./interface_pers/interface_pers.pri)
include (./interface_text/interface_text.pri)
include (./operation_event/operation_event.pri)
include (./operation_dialog/operation_dialog.pri)
include (./operation_global/operation_global.pri)
include (./output_figure/output_figure.pri)
include (./output_text_widget/output_text.pri)
include (./setting_analysis/setting_analysis.pri)
include (./setting_floor/setting_floor.pri)
include (./setting_frame/setting_frame.pri)
include (./setting_load/setting_load.pri)
include (./setting_material/setting_material.pri)
include (./setting_section/setting_section.pri)
include (./setting_view/setting_view.pri)
include (./setting_stresscombi/setting_stresscombi.pri)


RC_FILE = ./icon.rc


#for deploy
# 置換関数の宣言（使うところより前） windows対応
defineReplace(replaceCommand){
    DEPEND_FILE = $$1
    DEPEND_FILE ~= s|/|\|gi
    return($$DEPEND_FILE)
}


exists($$replaceCommand($$PWD/../deploy/$$BUILD_TYPE)){
    QMAKE_POST_LINK += rmdir /s /q  $$replaceCommand($$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
}
QMAKE_POST_LINK += mkdir $$replaceCommand($$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($${OUT_PWD}/$$BUILD_TYPE/$${TARGET}.exe $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($${PWD}/../libQGLViewer-2.9.1/QGLViewer/$${GLVIEWERLIBNAME}.dll $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($${P3DLIBROOT}/lib/$${P3DLIBNAME}.dll $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($${CALCULATIONMAKERLIBROOT}/lib/$${CALCULATIONMAKERLIBNAME}.dll $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($$dirname(QMAKE_QMAKE)/$${PRINTSUPPORTDLLNAME}.dll $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += copy /y $$replaceCommand($$dirname(QMAKE_QMAKE)/$${XMLDLLNAME}.dll $$PWD/../deploy/$$BUILD_TYPE)$$escape_expand(\n\t)
QMAKE_POST_LINK += $$replaceCommand($$dirname(QMAKE_QMAKE)/windeployqt.exe $$PWD/../deploy/$$BUILD_TYPE/$${TARGET}.exe)$$escape_expand(\n\t)
