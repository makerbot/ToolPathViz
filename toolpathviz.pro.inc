
#-------------------------------------------------
#
# QtCreator project include file
#
#-------------------------------------------------

QT       += opengl

INCLUDEPATH += $$GUI_SRC

SOURCES +=  $$GUI_SRC/main.cpp\
    $$GUI_SRC/mainwindow.cpp \
    $$GUI_SRC/gcode.cpp \
    $$GUI_SRC/gcodeview.cpp \
    $$GUI_SRC/gcodeviewapplication.cpp \
    $$GUI_SRC/arcball.cpp \
    $$GUI_SRC/quaternion.cpp \
    $$GUI_SRC/algebra3.cpp \


HEADERS  +=    $$GUI_SRC/mainwindow.h \
    $$GUI_SRC/gcode.h \
    $$GUI_SRC/gcodeview.h \
    $$GUI_SRC/gcodeviewapplication.h \
    $$GUI_SRC/arcball.h \
    $$GUI_SRC/algebra3.h \
    $$GUI_SRC/quaternion.h \
    $$GUI_SRC/glui_internal.h \


FORMS    += $$GUI_SRC/mainwindow.ui \

