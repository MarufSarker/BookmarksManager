#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include "bookmark.h"

//#include <mm/bookmarks/bookmarks.hh>

class BookmarkListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        identifierRole = Qt::UserRole,
        containerRole,
        typeRole,
        urlRole,
        titleRole,
        noteRole,
        createdRole,
        modifiedRole,
    };

    explicit BookmarkListModel(QObject *parent = nullptr);

    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    // mode: "container", "search"
//    void search(QString const& query, QString const& mode, QString const& parent);

    void selectIntoModel(QString const& query);
    void selectFromContainerIntoModel(QString const& query);

//    void test(QVariantList);
//    void test(QVariantMap);
//    void test(QList<QVariantMap>const&);
//    void test(Bookmark);

    bool insertBookmarks(QList<QVariantMap> const& data);
    bool updateBookmarks(QList<QVariantMap> const& data);
    bool deleteBookmarks(QList<QString> const& data);

    bool importFrom(QString const& from, QString const& path);
    bool vacuum();

private:
    QList<Bookmark*> mData;
//    mm::bookmarks::manager mManager {};
    QSqlDatabase mDb;

private:
    void convert(QObject* parent, QList<Bookmark*>& result, QList<QVariantMap> const& other) const;
};

#endif // BOOKMARKLISTMODEL_H
