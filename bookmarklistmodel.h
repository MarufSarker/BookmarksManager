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

    explicit BookmarkListModel(QObject *parent = nullptr, QString const& conName = "");

    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;

//    Q_INVOKABLE QVariantMap getMap(int const& index) const;
//    Q_INVOKABLE QList<QVariantMap> selectGetSelections() const;

    Q_INVOKABLE QString toLocalFile(QString const& path) const;

    Q_INVOKABLE void copyToClipboard(QString const& text) const;

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
    bool exportTo(QString const& path);


    void goInto(QString const& parent);
//    void goBack();
//    bool goBackable();
    void goHome();
    void goRefresh();
    QString currentContainer();

    void selectToggle(int const& index);
    bool selectSelected(int const& index);
    bool selectHasSelection();
    QList<QVariantMap> selectGetSelections();

    void cutSelections();
    bool cutHasSelection();
    bool cutPaste();

    QVariantMap getTypesCount();

    QString getDatabasePath();
//    bool setDatabaseDirectory(QString const& path);

//    void reopenDatabase(QString const& conName);
    void reopenDatabase();
    QSqlDatabase getDatabase();
//    void closeDatabase();
signals:
    void selectionsSizeChanged();
    void cutSizeChanged();

private:
    QList<Bookmark*> mData;
//    mm::bookmarks::manager mManager {};
//    QSqlDatabase mDb;
//    QString dbName = "mmBookmarks";

//    QList<QString> mParentsHistory;
    QString mCurrentContainer = "";

    QItemSelectionModel mSelModel;
    QList<QVariantMap> mCutModel;

    QString connectionName = "";

//    QString settingsOrg = "mm.bookmarks.manager";
//    QString settingsApp = "BookmarksManager";
//    QString settingsDir = "directory";

private:
    void convert(QObject* parent, QList<Bookmark*>& result, QList<QVariantMap> const& other) const;
    void clearSelections();
};

#endif // BOOKMARKLISTMODEL_H
