INCLUDEPATH += $$PWD

include ($$PWD/common/common.pri)
include ($$PWD/section01/section01.pri)
include ($$PWD/section02/section02.pri)
include ($$PWD/section03/section03.pri)
include ($$PWD/section04/section04.pri)
include ($$PWD/section05/section05.pri)
include ($$PWD/section06/section06.pri)
include ($$PWD/section07/section07.pri)
include ($$PWD/section08/section08.pri)
include ($$PWD/section09/section09.pri)
include ($$PWD/section10/section10.pri)
include ($$PWD/section11/section11.pri)

HEADERS += $$PWD/abstractwrite.h \
           $$PWD/buildinginformationwidget.h \
           $$PWD/calculation_customsettingdialog.h \
           $$PWD/calculation_manager_dialog.h \
           $$PWD/calculation_tabwidget.h \
           $$PWD/calculationpapersettingwidget.h \
           $$PWD/calculationreader.h \
           $$PWD/linecalculation.h \
           $$PWD/nposk.h \
           $$PWD/posk_plaintextedit.h \
           $$PWD/prcard.h \
           $$PWD/resorce_plaintextedit.h

SOURCES += $$PWD/abstractwrite.cpp \
           $$PWD/buildinginformationwidget.cpp \
           $$PWD/calculation_customsettingdialog.cpp \
           $$PWD/calculation_manager_dialog.cpp \
           $$PWD/calculation_tabwidget.cpp \
           $$PWD/calculationpapersettingwidget.cpp \
           $$PWD/calculationreader.cpp \
           $$PWD/linecalculation.cpp \
           $$PWD/nposk.cpp \
           $$PWD/posk_plaintextedit.cpp \
           $$PWD/prcard.cpp \
           $$PWD/resorce_plaintextedit.cpp

FORMS +=  
