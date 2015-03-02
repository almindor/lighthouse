#include <QDebug>
#include <QDir>
#include "languages.h"
#include "types.h"

namespace Lighthouse {

    Languages::Languages(const QGuiApplication* app) :
        QAbstractListModel(0), fSettings(), fList()
    {
        fApp = app;
        fTranslator = NULL;
        fTranslationDir = app->applicationDirPath() + "/../share/" + LNAME + "/translations/";

        // set locale if saved from settings
        const QString& presetLocale = fSettings.value("language/locale", QLocale::system().name()).toString();
        if ( presetLocale != QLocale::system().name() ) {
            fTranslator = new QTranslator();
            if ( fTranslator->load(QLocale(presetLocale), LNAME, "-", fTranslationDir) ) {
                fApp->installTranslator(fTranslator);
            } else {
                qWarning() << "Unable to load translation for " << fTranslationDir + LNAME + "-" + presetLocale + ".qm" << "\n";
            }
        }

        // find all applicable locales
        QDir dir(fTranslationDir);
        dir.setFilter(QDir::Files);
        //dir.setNameFilters(); // TODO to make sure
        QStringList files = dir.entryList();
        fList.append(QLocale::system());
        foreach ( QString f, files ) {
            const int n = f.indexOf('-');
            if ( n < 0 ) {
                continue;
            }
            const int m = f.indexOf('.');
            QStringRef locstr(&f, n + 1, m - n - 1);
            QLocale locale = QLocale(locstr.toString());
            if ( locale.name() != QLocale::system().name() ) {
                fList.append(locale);
            }
        }
    }

    Languages::~Languages() {
        if ( fTranslator != NULL ) {
            delete fTranslator;
        }
    }

    QHash<int, QByteArray> Languages::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[IndexRole] = "index";
        roles[LanguageRole] = "language";
        roles[CountryRole] = "country";
        return roles;
    }

    int Languages::rowCount(const QModelIndex & parent __attribute__ ((unused))) const {
        return fList.count();
    }

    QVariant Languages::data(const QModelIndex & index, int role __attribute__ ((unused))) const {
        switch ( role ) {
            case IndexRole: return index.row();
            case LanguageRole: return QLocale::languageToString(fList.at(index.row()).language());
            case CountryRole: return QLocale::countryToString(fList.at(index.row()).country());
        }

        return tr("Unknown", "language");
    }

    void Languages::selectLanguage(const int index) {
        fSettings.setValue("language/locale", fList.at(index).name());
    }

}
