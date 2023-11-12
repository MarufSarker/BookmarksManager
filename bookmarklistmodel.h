#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

#include <QAbstractListModel>
#include <QItemSelectionModel>
#include <QSqlDatabase>

#include "bookmark.h"

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

    Q_INVOKABLE QString toLocalFile(QString const& path) const;

    Q_INVOKABLE void copyToClipboard(QString const& text) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void selectIntoModel(QString const& query);
    void selectFromContainerIntoModel(QString const& query);

    bool insertBookmarks(QList<QVariantMap> const& data);
    bool updateBookmarks(QList<QVariantMap> const& data);
    bool deleteBookmarks(QList<QString> const& data);

    bool importFrom(QString const& from, QString const& path);
    bool vacuum();
    bool exportTo(QString const& path);

    void goInto(QString const& parent);
    void goHome();
    void goRefresh();
    QString getCurrentContainer();

    void selectToggle(int const& index);
    bool selectSelected(int const& index);
    bool selectHasSelection();
    QList<QVariantMap> selectGetSelections();

    void cutSelections();
    bool cutHasSelection();
    bool cutPaste();

    QVariantMap getTypesCount();

    QString getDatabasePath();

    void reopenDatabase();
    QSqlDatabase getDatabase();

signals:
    void selectionsSizeChanged();
    void cutSizeChanged();

private:
    QList<Bookmark*> dataList;
    QString currentContainer = "";
    QItemSelectionModel selectionModel;
    QList<QVariantMap> cutModel;
    QString connectionName = "";

private:
    void convert(QObject* parent, QList<Bookmark*>& result, QList<QVariantMap> const& other) const;
    void clearSelections();
};

#endif // BOOKMARKLISTMODEL_H
