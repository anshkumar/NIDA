TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    imageanalysis.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    imageanalysis.h \
    httpclient.h

# add open CV
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

unix: PKGCONFIG += qjsonrpc
