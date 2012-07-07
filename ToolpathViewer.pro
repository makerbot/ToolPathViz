QT       += core gui opengl

TARGET = ToolpathViewer
TEMPLATE = app
#TEMPLATE = lib

HEADERS += \
    src/model/toolpath.h \
    src/model/visual.h \
    src/model/step.h \
    src/load/parsers.h \
    src/load/visualizers.h \
    src/load/gcode/gcode.h \
    src/view/toolpathscene.h \
    src/view/viewmodel.h \
    src/example.h

SOURCES += src/main.cpp \
    src/model/toolpath.cpp \
    src/model/visual.cpp \
    src/load/parsers.cpp \
    src/load/visualizers.cpp \
    src/load/gcode/gcode.cpp \
    src/view/toolpathscene.cpp \
    src/view/viewmodel.cpp
