#include "bookmarklistmodel.h"
#include "sql.hh"
#include <QtSql>

BookmarkListModel::BookmarkListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    // mManager.open("./");
//    mManager.open("/home/cube/Downloads/qt-projects/BookmarksManager/", "mm_bookmarks.db");

    QString dbPath ("/home/cube/Downloads/qt-projects/BookmarksManager/mm_bookmarks.db");
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(dbPath);
    if (!mDb.open())
    {
        qDebug() << mDb.lastError();
        return;
    }
    // prepare
    QSqlQuery query(mDb);
    try
    {
        for (auto const& v : mm::bookmarks::sql::versions::create)
        {
            query.prepare(QString::fromStdString(v));
            query.exec();
            query.finish();
        }
        for (auto const& v : mm::bookmarks::sql::bookmarks::create)
        {
            query.prepare(QString::fromStdString(v));
            query.exec();
            query.finish();
        }
    }
    catch (std::runtime_error const& e)
    {
        qDebug() << e.what();
        mDb.close();
    }
}

int BookmarkListModel::rowCount(QModelIndex const& parent) const
{
    return (parent.isValid()) ? 0 : mData.count();
}

QVariant BookmarkListModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
        return {};
    switch (role)
    {
    case identifierRole:
        return QVariant(mData.at(index.row())->identifier);
    case containerRole:
        return QVariant(mData.at(index.row())->container);
    case typeRole:
        return QVariant(mData.at(index.row())->type);
    case urlRole:
        return QVariant(mData.at(index.row())->url);
    case titleRole:
        return QVariant(mData.at(index.row())->title);
    case noteRole:
        return QVariant(mData.at(index.row())->note);
    case createdRole:
        return QVariant(mData.at(index.row())->created);
    case modifiedRole:
        return QVariant(mData.at(index.row())->modified);
    default:
        return {};
    }
}

QHash<int, QByteArray> BookmarkListModel::roleNames() const
{
    static QHash<int, QByteArray> const roles {
        {identifierRole, "identifier"},
        {containerRole, "container"},
        {typeRole, "type"},
        {urlRole, "url"},
        {titleRole, "title"},
        {noteRole, "note"},
        {createdRole, "created"},
        {modifiedRole, "modified"},
    };
    return roles;
}

//void BookmarkListModel::search(QString const& query, QString const& mode, QString const& parent)
//{
//    beginRemoveRows(QModelIndex(), 0, mData.size() - 1);
//    mData.clear();
//    endRemoveRows();

//    if (query.isEmpty() || !mManager.opened())
//        return;

//    static QString const modeCont = "container";
//    static QString const modeSrch = "search";

//    if (mode != modeCont && mode != modeSrch)
//        return;

//    try
//    {
//        if (mode == modeCont)
//        {
//            mm::bookmarks::comparison comp {mm::bookmarks::similarity_type::EQUAL,
//                                           "container", query.toStdString()};
//            auto amount = mManager.count_bookmarks(comp);
//            auto _data = mManager.select_bookmarks(comp, {{"type", true}}, amount, 0);

//            for (auto i = 0; i < _data.size(); ++i)
//                mData.append(_data.at(i));

//            if (!parent.isEmpty() /*!(parent == "0" && query == "0")*/) {
//                mm::bookmarks::bookmark p {};
//                p.identifier = parent.toStdString();
//                p.title = "../";
//                p.type = "CONTAINER";
//                qDebug() << p.identifier;
//                mData.prepend(p);
//            }
//        }
//    }
//    catch (const std::exception& e)
//    {
//        qDebug() << e.what();
//    }

//    beginInsertRows(QModelIndex(), 0, mData.size() - 1);
//    endInsertRows();
//}

void queryDb(QObject* parent, QSqlQuery& query, QList<Bookmark*>& data)
{
    if (!query.exec())
    {
        qDebug() << query.lastError();
        query.finish();
        return;
    }

    int idIdentifier = query.record().indexOf("identifier");
    int idContainer  = query.record().indexOf("container");
    int idType       = query.record().indexOf("type");
    int idUrl        = query.record().indexOf("url");
    int idTitle      = query.record().indexOf("title");
    int idNote       = query.record().indexOf("note");
    int idCreated    = query.record().indexOf("created");
    int idModified   = query.record().indexOf("modified");

    while (query.next())
    {
        Bookmark* datum = new Bookmark(parent);
        datum->identifier = query.value(idIdentifier).toString();
        datum->container = query.value(idContainer).toString();
        datum->type = query.value(idType).toString();
        datum->url = query.value(idUrl).toString();
        datum->title = query.value(idTitle).toString();
        datum->note = query.value(idNote).toString();
        datum->created = query.value(idCreated).toString();
        datum->modified = query.value(idModified).toString();
        data.append(datum);
    }

    query.finish();
}

void BookmarkListModel::selectIntoModel(QString const& _query)
{
    beginRemoveRows(QModelIndex(), 0, mData.size() - 1);
    mData.clear();
    endRemoveRows();

    if (_query.isEmpty() || !mDb.isOpen())
        return;

    QSqlQuery query(mDb);
    query.prepare("SELECT * FROM mm_bookmarks WHERE [url] LIKE (:url) OR [title] LIKE (:title)");
    query.bindValue(":url", "%" + _query + "%");
    query.bindValue(":title", "%" + _query + "%");
    queryDb(this, query, mData);

    beginInsertRows(QModelIndex(), 0, mData.size() - 1);
    endInsertRows();
}

void BookmarkListModel::selectFromContainerIntoModel(QString const& _query)
{
    beginRemoveRows(QModelIndex(), 0, mData.size() - 1);
    mData.clear();
    endRemoveRows();

    if (_query.isEmpty() || !mDb.isOpen())
        return;

    QSqlQuery query(mDb);

//    if (reverse)
//    {
//        query.prepare("SELECT * FROM mm_bookmarks WHERE [container] = "
//                      "(SELECT [container] FROM mm_bookmarks WHERE [identifier] = (:identifier));");
//        query.bindValue(":identifier", _query);
//        queryDb(this, query, mData);
//    }
//    else
//    {
    query.prepare("SELECT * FROM mm_bookmarks WHERE [container] = (:container)");
    query.bindValue(":container", _query);
    queryDb(this, query, mData);
//    }

    beginInsertRows(QModelIndex(), 0, mData.size() - 1);
    endInsertRows();
}

bool BookmarkListModel::insertBookmarks(QList<Bookmark> data)
{
    if (!mDb.isOpen())
        return false;

    QString sql = "INSERT INTO mm_bookmarks ([container], [type], [url], [title], [note]) VALUES ";
    QMap<QString, QString> bindings {};

    for (size_t i = 0; i < data.size(); ++i)
    {
        Bookmark const* bm = &data.at(i);

        if (bm->container.isEmpty())
            bm->container.toStdString() = mm::bookmarks::sql::bookmarks::helpers::defaults::container;

        if (bm->type.toStdString() != mm::bookmarks::sql::bookmarks::helpers::type::container &&
            bm->type.toStdString() != mm::bookmarks::sql::bookmarks::helpers::type::url)
            return false;

        QString _sql {};
        QString _i = QString::number(i);

        _sql += "(";
        _sql += ":container" + _i + ", ";
        _sql += ":type" + _i + ", ";
        _sql += ":url" + _i + ", ";
        _sql += ":title" + _i + ", ";
        _sql += ":note" + _i + "";
        _sql += ")";
        _sql += ((i + 1) < data.size()) ? ", " : "";

        sql += _sql;

        bindings[":container" + _i] = bm->container;
        bindings[":type" + _i] = bm->type;
        bindings[":url" + _i] = bm->url;
        bindings[":title" + _i] = bm->title;
        bindings[":note" + _i] = bm->note;
    }

    sql += ";";

    QSqlQuery query(mDb);
    query.prepare(sql);

    for (auto [key, value] : bindings.asKeyValueRange())
        query.bindValue(key, value);

    if (!query.exec())
    {
        qDebug() << query.lastError();
        query.finish();
        return false;
    }

    query.finish();
    return true;
}

bool BookmarkListModel::updateBookmarks(QList<Bookmark> data)
{
    if (!mDb.isOpen())
        return false;
    if (data.isEmpty())
        return true;

    for (size_t i = 0; i < data.size(); ++i)
    {
        auto* bm = &data.at(i);

        QString sql = "UPDATE mm_bookmarks SET";
        sql += " [container] = :container";
        sql += ", [url] = :url";
        sql += ", [title] = :title";
        sql += ", [note] = :note";
        sql += " WHERE [identifier] == :identifier;";

        QSqlQuery query(sql, mDb);

        query.bindValue(":container", bm->container);
        query.bindValue(":url", bm->url);
        query.bindValue(":title", bm->title);
        query.bindValue(":note", bm->note);
        query.bindValue(":identifier", bm->identifier);

        if (!query.exec())
        {
            qDebug() << query.lastError();
            query.finish();
            return false;
        }

        query.finish();

        if ((i + 1) == data.size())
            return true;
    }

    return false;
}

bool BookmarkListModel::deleteBookmarks(QList<QString> data)
{
    if (!mDb.isOpen())
        return false;
    if (data.isEmpty())
        return true;

    QString sql = "DELETE FROM mm_bookmarks WHERE ";
    QMap<QString, QString> bindings;

    for (int i = 0; i < data.size(); ++i)
    {
        QString id = data.at(i);
        QString _i = QString::number(i);
        if (i > 0)
            sql += " OR ";
        sql += "[identifier] = :identifier" + _i;
        if (i + 1 == data.size())
            sql += ";";
        bindings[":identifier" + _i] = id;
    }

    QSqlQuery query(sql, mDb);

    for (auto [key, value] : bindings.asKeyValueRange())
        query.bindValue(key, value);

    if (!query.exec())
    {
        qDebug() << query.lastError();
        query.finish();
        return false;
    }

    query.finish();
    return true;
}

bool BookmarkListModel::importFrom(QString const& from, QString const& path)
{
    static QString const typeFF = "FIREFOX_SQLITE";
    static QString const typeMM = "MMBOOKMARKS";

    if (!mDb.isOpen())
        return false;
    if (from != typeFF && from != typeMM)
        return false;
    if (path.isEmpty())
        return false;

    QList<QString> cleanup;
    QString detach;
    QString attach;
    QList<QString> preparation;
    QList<QString> process;

    if (from == typeMM)
    {
        namespace ns = mm::bookmarks::sql::imports::mm_bookmarks;
        for (auto const& v : ns::cleanup)
            cleanup.append(QString::fromStdString(v));
        detach = QString::fromStdString(ns::detach);
        attach = QString::fromStdString(ns::attach);
        for (auto const& v : ns::preparation)
            preparation.append(QString::fromStdString(v));
        for (auto const& v : ns::process)
            process.append(QString::fromStdString(v));
    }
    else if (from == typeFF)
    {
        namespace ns = mm::bookmarks::sql::imports::firefox_places_sqlite;
        for (auto const& v : ns::cleanup)
            cleanup.append(QString::fromStdString(v));
        detach = QString::fromStdString(ns::detach);
        attach = QString::fromStdString(ns::attach);
        for (auto const& v : ns::preparation)
            preparation.append(QString::fromStdString(v));
        for (auto const& v : ns::process)
            process.append(QString::fromStdString(v));
    }
    else
        return false;

    static auto _import_cleanup = [](QSqlDatabase& db, QList<QString> const& cl, QString const& dt)
    {
        // cleanup
        for (auto const& v : cl)
        {
            QSqlQuery query(v, db);
            if (!query.exec())
                qDebug() << query.lastError();
            query.finish();
        }
        // detach
        QSqlQuery query(dt, db);
        if (!query.exec())
            qDebug() << query.lastError();
        query.finish();
    };

    static auto _import_prepare_and_process = [](QSqlDatabase& db, QString const& att, QString const& path_,
                                                 QList<QString> const& prep, QList<QString> const& proc)
    {
        // prepare
        QString _att = att.arg(path_);
        _att.replace("'", "\\'");
        _att.replace("\"", "\\\"");
        _att.replace(";", "\\;");

        QSqlQuery query(_att, db);
        if (!query.exec())
            qDebug() << query.lastError();
        query.finish();

        for (auto const& v : prep)
        {
            QSqlQuery query(v, db);
            if (!query.exec())
                qDebug() << query.lastError();
            query.finish();
        }

        // process
        for (auto const& v : proc)
        {
            QSqlQuery query(v, db);
            if (!query.exec())
                qDebug() << query.lastError();
            query.finish();
        }
    };

    _import_cleanup(mDb, cleanup, detach);
    _import_prepare_and_process(mDb, attach, path, preparation, process);
    _import_cleanup(mDb, cleanup, detach);

    return true;
}

bool BookmarkListModel::vacuum()
{
    if (!mDb.isOpen())
        return false;
    QSqlQuery query("VACUUM;", mDb);
    return query.exec();
}
