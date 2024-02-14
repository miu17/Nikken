INCLUDEPATH += $$PWD

HEADERS += $$PWD/define_npstn.h \
           $$PWD/handle_npstn.h \
           $$PWD/handle_pstn.h \
    $$PWD/interpreter_csv.h \
           $$PWD/pstn_optional_data.h \
           $$PWD/read_from_bld3d.h \
           $$PWD/read_from_psv.h \

SOURCES += $$PWD/handle_pstn.cpp \
           $$PWD/handle_npstn.cpp \
    $$PWD/interpreter_csv.cpp \
           $$PWD/read_from_bld3d.cpp \
           $$PWD/read_from_psv.cpp \


