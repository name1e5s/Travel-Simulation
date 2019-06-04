TEMPLATE = app

QT += qml quick quickcontrols2

CONFIG += c++11

SOURCES += main.cpp \
    city_graph.cpp \
    graph_handler.cpp

RESOURCES += \  
    res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

DISTFILES += \
    MainWindow.qml

HEADERS += \ 
    city_graph.h \
    graph_handler.h
