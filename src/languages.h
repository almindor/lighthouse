#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QList>
#include <QSettings>
#include <QLocale>
#include <QStringList>
#include <QGuiApplication>
#include <QTranslator>
#include <QAbstractListModel>

namespace Lighthouse {

    class Languages : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum LanguageRoles {
            IndexRole = Qt::UserRole + 1,
            LanguageRole,
            CountryRole
        };

        explicit Languages(const QGuiApplication* app);
        ~Languages();
        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        Q_INVOKABLE void selectLanguage(const int index);
    private:
        QSettings fSettings;
        const QGuiApplication* fApp;
        QTranslator* fTranslator;
        QString fTranslationDir;
        QList<QLocale> fList;

    signals:

    public slots:

    };

}

#endif // LANGUAGES_H
