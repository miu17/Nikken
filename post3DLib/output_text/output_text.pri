INCLUDEPATH += $$PWD

include ($$PWD/01_definition/01_definition.pri)
include ($$PWD/02_input/02_input.pri)
include ($$PWD/03_prem/03_prem.pri)
include ($$PWD/04_pres/04_pres.pri)
include ($$PWD/05_prea/05_prea.pri)
include ($$PWD/06_sols/06_sols.pri)
include ($$PWD/07_stec/07_stec.pri)
include ($$PWD/08_chck/08_chck.pri)
include ($$PWD/09_qdcv/09_qdcv.pri)
include ($$PWD/10_result/10_result.pri)


HEADERS +=  \
    $$PWD/abstracttext.h \
    $$PWD/line_outputtext.h \
    $$PWD/outputtext_reader.h \
    $$PWD/outputtextparser.h

SOURCES +=  \
    $$PWD/abstracttext.cpp \
    $$PWD/line_outputtext.cpp \
    $$PWD/outputtext_reader.cpp \
    $$PWD/outputtextparser.cpp



