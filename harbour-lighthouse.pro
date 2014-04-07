# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-lighthouse

CONFIG += sailfishapp

QT += dbus

SOURCES += \
    src/cpu.cpp \
    src/memory.cpp \
    src/procreader.cpp \
    src/linehandler.cpp \
    src/types.cpp \
    src/process.cpp \
    src/monitor.cpp \
    src/lighthouse.cpp \
    src/battery.cpp
    src/battery.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/Summary.qml \
    qml/pages/CPU.qml \
    qml/pages/Settings.qml \
    qml/pages/Memory.qml \
    qml/pages/About.qml \
    qml/pages/Process.qml \
    qml/pages/Phone.qml \
    lighthouse.desktop \
    qml/Lighthouse.qml \
    rpm/harbour-lighthouse.spec \
    rpm/harbour-lighthouse.yaml
    qml/pages/Battery.qml
    qml/pages/Battery.qml
    harbour-lighthouse.pro

HEADERS += \
    src/cpu.h \
    src/memory.h \
    src/procreader.h \
    src/linehandler.h \
    src/types.h \
    src/process.h \
    src/monitor.h \
    src/battery.h
