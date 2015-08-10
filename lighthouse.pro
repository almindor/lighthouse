# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = lighthouse

TRANSLATIONS = \
    translations/lighthouse-en.ts \
    translations/lighthouse-cs.ts \
    translations/lighthouse-fr.ts \
    translations/lighthouse-ru.ts \
    translations/lighthouse-es.ts \
    translations/lighthouse-es_AR.ts \
    translations/lighthouse-sv.ts \
    translations/lighthouse-it.ts \
    translations/lighthouse-nb.ts \
    translations/lighthouse-nl.ts \
    translations/lighthouse-zh_CN.ts \
    translations/lighthouse-zh_TW.ts

CONFIG += \
    sailfishapp \
    sailfishapp_i18n

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
    src/battery.cpp \
    src/languages.cpp

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
    qml/InfoPopup.qml \
    qml/pages/Details.qml \
    qml/components/ProcIndicator.qml \
    qml/components/DoubleIndicator.qml \
    qml/components/UsageGraph.qml \
    qml/components/UsageHelper.js

HEADERS += \
    src/cpu.h \
    src/memory.h \
    src/procreader.h \
    src/linehandler.h \
    src/types.h \
    src/process.h \
    src/monitor.h \
    src/battery.h \
    src/languages.h

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
    qml/InfoPopup.qml
}
