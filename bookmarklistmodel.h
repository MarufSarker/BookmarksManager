#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

#include <QAbstractListModel>
#include <QItemSelectionModel>
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
        selectedRole,
        cutRole,
    };

    explicit BookmarkListModel(QObject *parent = nullptr);

    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QVariantMap getMap(int const& index) const;

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


    void goInto(QString const& parent);
//    void goBack();
//    bool goBackable();
    void goHome();
    void goRefresh();
    QString currentContainer();

    void selectToggle(int const& index);
    bool selectSelected(int const& index);

private:
    QList<Bookmark*> mData;
//    mm::bookmarks::manager mManager {};
    QSqlDatabase mDb;

//    QList<QString> mParentsHistory;
    QString mCurrentContainer = "";

private:
    void convert(QObject* parent, QList<Bookmark*>& result, QList<QVariantMap> const& other) const;
};

#endif // BOOKMARKLISTMODEL_H
