TEMPLATE = app

QT += qml quick quickcontrols2 positioning location

CONFIG += c++11

SOURCES += main.cpp \
    city_graph.cpp \
    graph_handler.cpp \
    transcontroller.cpp

RESOURCES += \  
    res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

DISTFILES += \
    MainWindow.qml

HEADERS += \ 
    city_graph.h \
    graph_handler.h \
    transcontroller.h

LIBS += -LD:\OpenSSL\lib -llibcrypto -llibssl

win32:RC_ICONS += logo.ico

# For GNU Compiler Collection.
*-g++* {
    QMAKE_CXXFLAGS += -Ofast -frename-registers -fopenmp -D_GLIBCXX_PARALLEL -funroll-loops -malign-double -minline-all-stringops -ftree-parallelize-loops=8 -flto -fuse-linker-plugin
}

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
