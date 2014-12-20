# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = lighthouse

CONFIG += sailfishapp

QT += dbus

TRANSLATIONS = \
    i18n/lighthouse.ts \
    i18n/lighthouse_ru.ts \
    i18n/lighthouse_es.ts \
    i18n/lighthouse_es_ar.ts

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
    rpm/lighthouse.yaml \
    qml/pages/Summary.qml \
    qml/pages/CPU.qml \
    qml/pages/Settings.qml \
    qml/pages/Memory.qml \
    qml/pages/About.qml \
    qml/pages/Process.qml \
    rpm/lighthouse.spec \
    qml/pages/Phone.qml \
    lighthouse.desktop \
    qml/Lighthouse.qml \
    qml/pages/Battery.qml \
    qml/InfoPopup.qml
    qml/pages/Battery.qml

HEADERS += \
    src/cpu.h \
    src/memory.h \
    src/procreader.h \
    src/linehandler.h \
    src/types.h \
    src/process.h \
    src/monitor.h \
    src/battery.h

lupdate_only {
SOURCES += \
    qml/cover/CoverPage.qml \
    qml/pages/Summary.qml \
    qml/pages/CPU.qml \
    qml/pages/Settings.qml \
    qml/pages/Memory.qml \
    qml/pages/About.qml \
    qml/pages/Process.qml \
    qml/pages/Phone.qml \
    qml/Lighthouse.qml \
    qml/pages/Battery.qml \
    qml/InfoPopup.qml \
    qml/pages/Battery.qml \
}
