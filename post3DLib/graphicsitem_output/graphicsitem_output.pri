INCLUDEPATH += $$PWD

include ($$PWD/face_length/face_length.pri)
include ($$PWD/numeric/numeric.pri)
include ($$PWD/axial/axial.pri)
include ($$PWD/stress/stress.pri)
include ($$PWD/map/map.pri)
include ($$PWD/model/model.pri)
include ($$PWD/deformation/deformation.pri)


HEADERS += \
           $$PWD/figure_template.h \
           $$PWD/figure_template_elevation.h \
           $$PWD/figure_template_factory.h \
           $$PWD/figure_template_plan.h \
           $$PWD/output_graphicsitem.h \
           $$PWD/output_jointitem.h \
           $$PWD/output_lineitem.h \
           $$PWD/output_lineplaneitem.h \
           $$PWD/output_linesectionitem.h \
           $$PWD/output_planeitem.h \
           $$PWD/output_point_item.h \
           $$PWD/output_springitem.h \
           $$PWD/output_text_item.h

SOURCES += \
           $$PWD/figure_template.cpp \
           $$PWD/figure_template_elevation.cpp \
           $$PWD/figure_template_factory.cpp \
           $$PWD/figure_template_plan.cpp \
           $$PWD/output_graphicsitem.cpp \
           $$PWD/output_jointitem.cpp \
           $$PWD/output_lineitem.cpp \
           $$PWD/output_lineplaneitem.cpp \
           $$PWD/output_linesectionitem.cpp \
           $$PWD/output_planeitem.cpp \
           $$PWD/output_point_item.cpp \
           $$PWD/output_springitem.cpp \
           $$PWD/output_text_item.cpp

DISTFILES += \
    $$PWD/map/map.pri


