QT += quick \
      multimedia

CONFIG += c++11 qml_debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/include
LIBS +=-L/usr/lib -lwiringPi

SOURCES += \
        main.cpp \
        timer.cpp \
        wirpi.cpp

RESOURCES += qml.qrc \
        icons/alarm.png \
        icons/down.png \
        icons/up.png \
        icons/set.png \
        icons/setup.png \
        icons/sleep.png \
        icons/snooze.png \
        icons/alarm1b.png \
        icons/alarm2b.png \
        icons/alarm1g.png \
        icons/alarm2g.png \
        icons/volume.png \
        sound/alarm1.wav \
        fonts/digital7.ttf

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    timer.h \
    wirpi.h

DISTFILES +=
