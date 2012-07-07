QT       += core gui opengl

TARGET = ToolpathViewer
TEMPLATE = app
#TEMPLATE = lib

HEADERS += \
    src/view/algebra3.h \
    src/view/arcball.h \
    src/view/quaternion.h \
    src/view/toolpathview.h \
    src/model/toolpath.h \
    src/model/visual.h \
    src/model/step.h \
    src/load/parsers.h \
    src/load/visualizers.h

SOURCES += src/main.cpp \
    src/view/algebra3.cpp \
    src/view/arcball.cpp \
    src/view/quaternion.cpp \
    src/view/toolpathview.cpp \
    src/model/toolpath.cpp \
    src/model/visual.cpp
