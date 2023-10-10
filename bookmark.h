#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QObject>
//#include <QtQmlIntegration/qqmlintegration.h>

class Bookmark : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString identifier MEMBER identifier)
//    Q_PROPERTY(QString container MEMBER container)
//    Q_PROPERTY(QString type MEMBER type)
//    Q_PROPERTY(QString url MEMBER url)
//    Q_PROPERTY(QString title MEMBER title)
//    Q_PROPERTY(QString note MEMBER note)
//    Q_PROPERTY(QString created MEMBER created)
//    Q_PROPERTY(QString modified MEMBER modified)
//    QML_ELEMENT
public:
    explicit Bookmark(QObject *parent = nullptr);

    QString identifier = {};
    QString container  = {};
    QString type       = {};
    QString url        = {};
    QString title      = {};
    QString note       = {};
    QString created    = {};
    QString modified   = {};
    bool    selected   = false;
    bool    cut        = false;

    QVariantMap asVariantMap() const;

signals:

};

#endif // BOOKMARK_H
