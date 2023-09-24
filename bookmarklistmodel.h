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

    bool insertBookmarks(QList<Bookmark> data);
    bool updateBookmarks(QList<Bookmark> data);
    bool deleteBookmarks(QList<QString> data);

    bool importFrom(QString const& from, QString const& path);
    bool vacuum();

private:
    QList<Bookmark*> mData;
//    mm::bookmarks::manager mManager {};
    QSqlDatabase mDb;

private:

};

#endif // BOOKMARKLISTMODEL_H
