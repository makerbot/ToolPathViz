QT       += core gui opengl

HEADERS += \
    $$PWD/src/model/toolpath.h \
    $$PWD/src/model/visual.h \
    $$PWD/src/model/step.h \
    $$PWD/src/load/parsers.h \
    $$PWD/src/load/visualizers.h \
    $$PWD/src/load/controlledvisuals.h \
    $$PWD/src/load/gcode/gcode.h \
    $$PWD/src/view/toolpathscene.h \
    $$PWD/src/view/viewmodel.h \
    $$PWD/src/view/viewpointcontroller.h

SOURCES += \
    $$PWD/src/model/toolpath.cpp \
    $$PWD/src/model/visual.cpp \
    $$PWD/src/load/parsers.cpp \
    $$PWD/src/load/visualizers.cpp \
    $$PWD/src/load/controlledvisuals.cpp \
    $$PWD/src/load/gcode/gcode.cpp \
    $$PWD/src/view/toolpathscene.cpp \
    $$PWD/src/view/viewmodel.cpp \
    $$PWD/src/view/viewpointcontroller.cpp
