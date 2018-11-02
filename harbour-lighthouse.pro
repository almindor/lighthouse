# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-lighthouse

TRANSLATIONS = \
    translations/harbour-lighthouse-ar.ts \
    translations/harbour-lighthouse-cs.ts \
    translations/harbour-lighthouse-da.ts \
    translations/harbour-lighthouse-de_DE.ts \
    translations/harbour-lighthouse-de_CH.ts \
    translations/harbour-lighthouse-el.ts \
    translations/harbour-lighthouse-en.ts \
    translations/harbour-lighthouse-es.ts \
    translations/harbour-lighthouse-es_AR.ts \
    translations/harbour-lighthouse-fi_FI.ts \
    translations/harbour-lighthouse-fr.ts \
    translations/harbour-lighthouse-gl.ts \
    translations/harbour-lighthouse-hu.ts \
    translations/harbour-lighthouse-it.ts \
    translations/harbour-lighthouse-nb.ts \
    translations/harbour-lighthouse-nl.ts \
    translations/harbour-lighthouse-pl.ts \
    translations/harbour-lighthouse-ru.ts \
    translations/harbour-lighthouse-sr.ts \
    translations/harbour-lighthouse-sv.ts \
    translations/harbour-lighthouse-tr.ts \
    translations/harbour-lighthouse-zh_CN.ts \
    translations/harbour-lighthouse-zh_TW.ts

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
    src/languages.cpp \
    src/zram.cpp

OTHER_FILES += \
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
    qml/pages/Details.qml \
    qml/components/ProcIndicator.qml \
    qml/components/DoubleIndicator.qml \
    qml/components/UsageGraph.qml \
    qml/components/UsageHelper.js \
    rpm/lighthouse.yaml \
    rpm/lighthouse.spec \
    harbour-lighthouse.desktop

HEADERS += \
    src/cpu.h \
    src/memory.h \
    src/procreader.h \
    src/linehandler.h \
    src/types.h \
    src/process.h \
    src/monitor.h \
    src/battery.h \
    src/languages.h \
    src/zram.h

icon86.files += icons/86x86/harbour-lighthouse.png
icon86.path = /usr/share/icons/hicolor/86x86/apps

icon108.files += icons/108x108/harbour-lighthouse.png
icon108.path = /usr/share/icons/hicolor/108x108/apps

icon128.files += icons/128x128/harbour-lighthouse.png
icon128.path = /usr/share/icons/hicolor/128x128/apps

icon256.files += icons/256x256/harbour-lighthouse.png
icon256.path = /usr/share/icons/hicolor/256x256/apps

INSTALLS += icon86 icon108 icon128 icon256

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
    qml/pages/Details.qml \
    qml/components/ProcIndicator.qml \
    qml/components/DoubleIndicator.qml \
    qml/components/UsageGraph.qml \
    qml/components/UsageHelper.js
}
