# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = Lighthouse

CONFIG += sailfishapp

SOURCES += src/Lighthouse.cpp \
    src/cpu.cpp \
    src/proc.cpp \
    src/memory.cpp

OTHER_FILES += qml/Lighthouse.qml \
    qml/cover/CoverPage.qml \
    rpm/Lighthouse.spec \
    rpm/Lighthouse.yaml \
    Lighthouse.desktop \
    qml/pages/Summary.qml \
    qml/pages/CPU.qml \
    qml/pages/Settings.qml \
    qml/pages/Memory.qml \
    qml/pages/About.qml

HEADERS += \
    src/cpu.h \
    src/proc.h \
    src/memory.h
