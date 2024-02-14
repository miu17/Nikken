INCLUDEPATH += $$PWD

HEADERS +=  \
    $$PWD/outputfigure_customsettingdialog.h \
    $$PWD/outputfigure_customsettingtabwidget.h \
    $$PWD/outputfigure_defaultsettingtabwidget.h \
    $$PWD/outputfigure_dialog.h \
    $$PWD/outputfigure_displaysettingtabwidget.h \
    $$PWD/outputfigure_plaintextedit.h \
    $$PWD/outputfigure_tabwidget.h \
    $$PWD/outputfigure_viewsettingwidget.h

SOURCES +=  \
    $$PWD/outputfigure_customsettingdialog.cpp \
    $$PWD/outputfigure_customsettingtabwidget.cpp \
    $$PWD/outputfigure_defaultsettingtabwidget.cpp \
    $$PWD/outputfigure_dialog.cpp \
    $$PWD/outputfigure_displaysettingtabwidget.cpp \
    $$PWD/outputfigure_plaintextedit.cpp \
    $$PWD/outputfigure_tabwidget.cpp \
    $$PWD/outputfigure_viewsettingwidget.cpp

include ($$PWD/widget/widget.pri)



