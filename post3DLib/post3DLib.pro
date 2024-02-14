#
TEMPLATE = lib
TARGET = post3DLib
DISTFILES += $$PWD/../beautifier/p3d.astylerc

DEPENDPATH += . debug
TRANSLATIONS = $$PWD/translate/post3DLib_ja.ts

RESOURCES += $$PWD/resource.qrc

QMAKE_CXXFLAGS += -MP
PRECOMPILED_HEADER = $$PWD/precompile.h
CONFIG += precompile_header

QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
QMAKE_CXXFLAGS_STL_ON = /EHa
#QMAKE_CXXFLAGS += /WX

CONFIG *= qt opengl c++11
QT *= opengl xml svg widgets printsupport svg concurrent

# for class export
DEFINES += POST3D_LIB

INCLUDEPATH *= $$PWD/../libQGLViewer-2.9.1

CONFIG( debug, debug|release ) {
    GLVIEWERLIBNAME = QGLViewerd2
    TARGET = $$join(TARGET,,,d)
    P3DLIBBUILDMODE = debug
}
CONFIG( release, debug|release ) {
    GLVIEWERLIBNAME = QGLViewer2
    P3DLIBBUILDMODE = release
}

#以下、intelコンパイラのifconsol.libに依存
LIBS += -l$$PWD/../NewBUILDING3D/3dcal/lib/3dcal_comm
LIBS += -l$$PWD/../NewBUILDING3D/3dcal/lib/3dcal_pos1
LIBS += -l$$PWD/../NewBUILDING3D/3dcal/lib/3dcal_etc
LIBS += -l$$PWD/../NewBUILDING3D/3dcal/lib/3dcal_qdcv
LIBS += -L"$$PWD/../fortranLib/IntelFortran/2021.4.0"

LIBS += -lOpenGL32
LIBS *= -L$$PWD/../libQGLViewer-2.9.1/QGLViewer -l$${GLVIEWERLIBNAME}
INCLUDEPATH += . $$PWD/../libQGLViewer-2.9.1

# Header files for install
headerFiles.path = $$PWD/include
headerFiles.files = \
                    $$PWD/analysis/abstract_reader.h \
                    $$PWD/analysis/abstractlinedata.h \
                    $$PWD/analysis/analysisconditionreader.h \
                    $$PWD/analysis/generallinecondition.h \
                    $$PWD/analysis/linecondition.h \
                    $$PWD/analysis/textdataunit.h \
                    $$PWD/calculation/calc_load.h \
                    $$PWD/calculation/calc_any_point_stress.h \
                    $$PWD/common/calc_vector2d.h \
                    $$PWD/common/calc_vector3d.h \
                    $$PWD/common/global_point.h \
                    $$PWD/common/parameters.h \
                    $$PWD/common/typicalcolor_style.h \
                    $$PWD/common/typicalcolor_table.h \
                    $$PWD/data_output/define_output.h \
                    $$PWD/data_output/member_force.h \
                    $$PWD/data_output/unified_output.h \
                    $$PWD/data_output/unified_calcload.h \
                    $$PWD/data_unified/define_analysis2dsettings.h \
                    $$PWD/data_unified/define_analysis3dsettings.h \
                    $$PWD/data_unified/define_bar_arrangement.h \
                    $$PWD/data_unified/define_calculationload2dsettings.h \
                    $$PWD/data_unified/define_calculationload3dsettings.h \
                    $$PWD/data_unified/define_defaultvalue.h \
                    $$PWD/data_unified/define_input2dsettings.h \
                    $$PWD/data_unified/define_input3dsettings.h \
                    $$PWD/data_unified/define_material_struct.h \
                    $$PWD/data_unified/define_section_struct.h \
                    $$PWD/data_unified/define_settings.h \
                    $$PWD/data_unified/define_unifieddata.h \
                    $$PWD/data_unified/factory_unifieddata.h \
                    $$PWD/data_unified/fixed_data.h \
                    $$PWD/data_unified/lineshape.h \
                    $$PWD/data_unified/manage_floordata.h \
                    $$PWD/data_unified/manage_framedata.h \
                    $$PWD/data_unified/manage_jointdata.h \
                    $$PWD/data_unified/manage_loaddata.h \
                    $$PWD/data_unified/manage_materialdata.h \
                    $$PWD/data_unified/manage_memberdata.h \
                    $$PWD/data_unified/manage_sectiondata.h \
                    $$PWD/data_unified/manage_springdata.h \
                    $$PWD/data_unified/n3d_uuidtable.h \
                    $$PWD/data_unified/operation_commands.h \
                    $$PWD/data_unified/unified_analysisdata.h \
                    $$PWD/data_unified/unified_data.h \
                    $$PWD/data_unified/unified_inputdata.h \
                    $$PWD/data_unified/unified_project.h \
                    $$PWD/data_unified/unified_settings.h \
                    $$PWD/data_unified/version.h \
                    $$PWD/draw_figure/define_draw_figure.h \
                    $$PWD/draw_figure/draw_calcload_figure.h \
                    $$PWD/draw_figure/draw_figure.h \
                    $$PWD/graphicsitem_input/customdraw_item.h \
                    $$PWD/graphicsitem_input/customdraw_hlineitem.h \
                    $$PWD/graphicsitem_input/customdraw_vlineitem.h \
                    $$PWD/graphicsitem_input/customdraw_hplaneitem.h \
                    $$PWD/graphicsitem_input/customdraw_vplaneitem.h \
                    $$PWD/graphicsitem_input/h_columnitem.h \
                    $$PWD/graphicsitem_input/v_columnitem.h \
                    $$PWD/graphicsitem_input/h_girderitem.h \
                    $$PWD/graphicsitem_input/v_girderitem.h \
                    $$PWD/graphicsitem_input/h_braceitem.h \
                    $$PWD/graphicsitem_input/v_braceitem.h \
                    $$PWD/graphicsitem_input/h_beamitem.h \
                    $$PWD/graphicsitem_input/v_beamitem.h \
                    $$PWD/graphicsitem_input/h_damperitem.h \
                    $$PWD/graphicsitem_input/v_damperitem.h \
                    $$PWD/graphicsitem_input/h_isoitem.h \
                    $$PWD/graphicsitem_input/v_isoitem.h \
                    $$PWD/graphicsitem_input/h_slabitem.h \
                    $$PWD/graphicsitem_input/v_slabitem.h \
                    $$PWD/graphicsitem_input/h_wallitem.h \
                    $$PWD/graphicsitem_input/v_wallitem.h \
                    $$PWD/graphicsitem_input/jointitem.h \
                    $$PWD/graphicsitem_input/jointloaditem.h \
                    $$PWD/graphicsitem_input/h_memberloaditem.h \
                    $$PWD/graphicsitem_input/h_slabloaditem.h \
                    $$PWD/graphicsitem_output/figure_template.h \
                    $$PWD/graphicsitem_output/figure_template_factory.h \
                    $$PWD/graphicsitem_output/output_graphicsitem.h \
                    $$PWD/graphicsitem_output/output_jointitem.h \
                    $$PWD/graphicsitem_output/output_lineitem.h \
                    $$PWD/graphicsitem_output/output_lineplaneitem.h \
                    $$PWD/graphicsitem_output/output_linesectionitem.h \
                    $$PWD/graphicsitem_output/output_planeitem.h \
                    $$PWD/graphicsitem_output/output_springitem.h \
                    $$PWD/graphicsitem_calcload/calcloadgraphicsitem.h \
                    $$PWD/handle_output/define_npstn.h \
                    $$PWD/handle_output/handle_pstn.h \
                    $$PWD/handle_output/handle_npstn.h \
                    $$PWD/handle_output/pstn_optional_data.h \
                    $$PWD/handle_output/read_from_bld3d.h \
                    $$PWD/handle_output/read_from_psv.h \
                    $$PWD/output_figure/line_outputfigure.h \
                    $$PWD/output_figure/outputfigure_reader.h \
                    $$PWD/output_text/abstracttext.h \
                    $$PWD/output_text/line_outputtext.h \
                    $$PWD/output_text/outputtextparser.h \
                    $$PWD/output_text/outputtext_reader.h \
                    $$PWD/scene_input/elevationCI_grscene.h \
                    $$PWD/scene_input/elevationUI_grscene.h \
                    $$PWD/scene_input/planCI_grscene.h \
                    $$PWD/scene_input/planUI_grscene.h \
                    $$PWD/scene_input/background_floor.h \
                    $$PWD/scene_input/background_frame.h \
                    $$PWD/scene_input/input_grscene.h \
                    $$PWD/scene_input/copy_background_floor.h \
                    $$PWD/scene_input/copy_background_frame.h \
                    $$PWD/scene_input/input_grscene.h \
                    $$PWD/scene_output/elevationOT_grscene.h \
                    $$PWD/scene_output/planOT_grscene.h \
                    $$PWD/scene_output/output_background_floor.h \
                    $$PWD/scene_output/output_background_frame.h \
                    $$PWD/scene_output/output_grscene.h \
                    $$PWD/scene_calcload/calcload_background_floor.h \
                    $$PWD/scene_calcload/calcload_background_frame.h \
                    $$PWD/scene_calcload/calcload_grscene.h \
                    $$PWD/scene_calcload/elevationCL_grscene.h \
                    $$PWD/scene_calcload/planCL_grscene.h \
                    $$PWD/scene_graph/define_graph.h \
                    $$PWD/scene_graph/graph_factory.h \
                    $$PWD/scene_graph/graph_scene.h \
                    $$PWD/scene_graph/graph_3d_scene.h \
                    $$PWD/scene_graph/graph_3d_view.h \
                    $$PWD/scene_pers/ascii_table.h \
                    $$PWD/scene_pers/define_glsetting.h \
                    $$PWD/scene_pers/persCL_glview.h \
                    $$PWD/scene_pers/persCL_partsdraw.h \
                    $$PWD/scene_pers/persCL_scene.h \
                    $$PWD/scene_pers/persOT_glview.h \
                    $$PWD/scene_pers/persOT_partsdraw.h \
                    $$PWD/scene_pers/persOT_scene.h \
                    $$PWD/scene_pers/persUI_glview.h \
                    $$PWD/scene_pers/persUI_scene.h \
                    $$PWD/scene_common/customdraw_grscene.h \
                    $$PWD/scene_common/base_background.h \


INSTALLS += headerFiles

# Library files for install
libraryFiles.path = $$PWD/lib
libraryFiles.files += $$OUT_PWD/$${P3DLIBBUILDMODE}/$${TARGET}.lib
libraryFiles.files += $$OUT_PWD/$${P3DLIBBUILDMODE}/$${TARGET}.dll
libraryFiles.CONFIG += no_check_exist
INSTALLS += libraryFiles

include (./post3DLib.pri)


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
