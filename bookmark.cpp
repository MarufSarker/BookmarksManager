#include "bookmark.h"

Bookmark::Bookmark(QObject *parent)
    : QObject{parent}
{
}

QVariantMap Bookmark::asVariantMap() const
{
    QVariantMap res;
    res.insert("identifier", identifier);
    res.insert("container", container);
    res.insert("type", type);
    res.insert("url", url);
    res.insert("title", title);
    res.insert("note", note);
    res.insert("created", created);
    res.insert("modified", modified);
    res.insert("selected", selected);
    res.insert("cut", cut);
    return res;
}
