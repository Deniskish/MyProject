QT       += core gui widgets opengl
QT += openglwidgets
# INCLUDEPATH += C:/Libraries/CGAL/include \
#                C:/Libraries/CGAL/auxiliary/gmp/include \
#                C:/Libraries/CGAL/auxiliary/gmp/lib
INCLUDEPATH += C:/vcpkg/installed/x64-mingw-dynamic/include

LIBS += -lopengl32 \
        -lglu32 \
        -LC:/vcpkg/installed/x64-mingw-dynamic/lib \
        -lgmp \
        -lmpfr
        # -LC:/vcpkg/installed/x64-mingw-dynamic/include/CGAL/export
        # -lCGAL


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # clipper2/clipper.engine.cpp \
    # clipper2/clipper.offset.cpp \
    # clipper2/clipper.rectclip.cpp \
    clipper.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp

HEADERS += \
    # clipper2/clipper.core.h \
    # clipper2/clipper.engine.h \
    # clipper2/clipper.export.h \
    # clipper2/clipper.h \
    # clipper2/clipper.minkowski.h \
    # clipper2/clipper.offset.h \
    # clipper2/clipper.rectclip.h \
    # clipper2/clipper.version.h \
    clipper.hpp \
    mainwindow.h \
    openglwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
