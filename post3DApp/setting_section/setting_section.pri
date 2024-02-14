INCLUDEPATH += $$PWD

include ($$PWD/column/section_col.pri)
include ($$PWD/girder/section_gir.pri)
include ($$PWD/beam/section_beam.pri)
include ($$PWD/brace/section_brace.pri)
include ($$PWD/isolator/section_isolator.pri)
include ($$PWD/base/section_base.pri)
include ($$PWD/damper/section_damper.pri)
include ($$PWD/opening/section_opening.pri)
include ($$PWD/wall/section_wall.pri)
include ($$PWD/uwall/section_uwall.pri)
include ($$PWD/slab/section_slab.pri)

HEADERS += $$PWD/define_sectiontable.h \
           $$PWD/section_calculations.h \
           $$PWD/section_customdialog.h \
           $$PWD/section_filterdialog.h \
           $$PWD/section_linebuttons.h \
           $$PWD/section_linecalculate.h \
           $$PWD/section_linedraw.h \
           $$PWD/section_tablemodel.h \
           $$PWD/section_tableview.h \
           $$PWD/section_tablewidget.h \
           $$PWD/standard_section.h \
           $$PWD/standard_section_combobox.h \
           $$PWD/standard_section_delegate.h

SOURCES += $$PWD/define_sectiontable.cpp \
           $$PWD/section_calculations.cpp \
           $$PWD/section_customdialog.cpp \
           $$PWD/section_filterdialog.cpp \
           $$PWD/section_linebuttons.cpp \
           $$PWD/section_linedraw.cpp \
           $$PWD/section_linecalculate.cpp \
           $$PWD/section_tablemodel.cpp \
           $$PWD/section_tableview.cpp \
           $$PWD/section_tablewidget.cpp \
           $$PWD/standard_section.cpp \
           $$PWD/standard_section_delegate.cpp

