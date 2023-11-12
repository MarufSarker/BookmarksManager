#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QObject>
#include <QVariantMap>

class Bookmark : public QObject
{
    Q_OBJECT
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
};

#endif // BOOKMARK_H
