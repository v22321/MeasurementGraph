QT += quick charts concurrent core-private

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        data_parser/abstractdatareader.cpp \
        data_parser/header.cpp \
        data_parser/measuredata.cpp \
        data_parser/ssddatareader.cpp \
        datacollector.cpp \
        graph/averagepointsadapter.cpp \
        graph/basepointsadapter.cpp \
        graph/graphmodel.cpp \
        graph.cpp \
        main.cpp \
        uiwrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    data_parser/abstractdatareader.h \
    data_parser/header.h \
    data_parser/measuredata.h \
    data_parser/ssddatareader.h \
    datacollector.h \
    graph.h \
    defines.h \
    graph/averagepointsadapter.h \
    graph/basepointsadapter.h \
    graph/graphmodel.h \
    graph/ipointsadapter.h \
    uiwrapper.h
