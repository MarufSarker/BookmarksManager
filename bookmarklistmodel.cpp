#include "bookmarklistmodel.h"
#include "sql.hh"
#include <QtSql>
#include <QTime>
#include <QSettings>
#include <QFileInfo>

BookmarkListModel::BookmarkListModel(QObject *parent)
    : QAbstractListModel{parent}, mSelModel{this}
{
    // mManager.open("./");
//    mManager.open("/home/cube/Downloads/qt-projects/BookmarksManager/", "mm_bookmarks.db");

    reopenDatabase();

//    QString dbDir = getDatabaseDirectory();
//    if (dbDir.isEmpty())
//        return;
//    QString dbPath = dbDir + "mm_bookmarks.db";
//    reopenDatabase(dbPath);

////    QString dbPath ("/home/cube/Downloads/qt-projects/BookmarksManager/mm_bookmarks.db");
//    mDb = QSqlDatabase::addDatabase("QSQLITE");
//    mDb.setDatabaseName(dbPath);
//    if (!mDb.open())
//    {
//        qDebug() << mDb.lastError();
//        return;
//    }
//    // prepare
//    QSqlQuery query(mDb);
//    try
//    {
//        for (auto const& v : mm::bookmarks::sql::versions::create)
//        {
//            query.prepare(QString::fromStdString(v));
//            query.exec();
//            query.finish();
//        }
//        for (auto const& v : mm::bookmarks::sql::bookmarks::create)
//        {
//            query.prepare(QString::fromStdString(v));
//            query.exec();
//            query.finish();
//        }
//    }
//    catch (std::runtime_error const& e)
//    {
//        qDebug() << e.what();
//        mDb.close();
//    }
}

void BookmarkListModel::reopenDatabase()
{
//    QString dbDir = getDatabaseDirectory();
//    if (dbDir.isEmpty())
//        return;
//    QString dbPath = dbDir + "/mm_bookmarks.db";
//    reopenDatabase(dbPath);
    reopenDatabase(getDatabasePath());
}

void BookmarkListModel::reopenDatabase(QString const& path)
{
    qDebug() << "Opening Database:" << path;
    if (mDb.isOpen())
        mDb.close();
    //    QString dbPath ("/home/cube/Downloads/qt-projects/BookmarksManager/mm_bookmarks.db");
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(path);
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
    case selectedRole:
        return QVariant(mData.at(index.row())->selected);
    case cutRole:
        return QVariant(mData.at(index.row())->cut);
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
        {selectedRole, "selected"},
        {cutRole, "cut"},
    };
    return roles;
}

//QVariantMap BookmarkListModel::getMap(int const& index) const
//{
//    return mData.at(index)->asVariantMap();
//}


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

void queryDb(QObject* parent, QSqlQuery& query, QList<Bookmark*>& data, QList<QVariantMap>const& cuts = {})
{
    if (!query.exec())
    {
        qDebug() << query.lastError() << query.executedQuery();
        query.finish();
        return;
    }

    QList<QString> cutIds;

    for (auto v : cuts)
        cutIds.append(v["identifier"].toString());

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
        datum->selected = false;
        datum->cut = cutIds.contains(datum->identifier);
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
    query.prepare("SELECT * FROM mm_bookmarks WHERE [url] LIKE (:url) OR [title] LIKE (:title) ORDER BY [title]");
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

    mCurrentContainer = _query;

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
    // query.prepare("SELECT * FROM mm_bookmarks WHERE [container] = (:container)");
    // query.bindValue(":container", _query);
    // queryDb(this, query, mData);
//    }

    qDebug() << "[container]" << _query;

    query.prepare(R"EOF(
    SELECT * FROM (
        SELECT
            [identifier],
            [container],
            [type],
            [url],
            '../' AS [title],
            [note],
            [created],
            [modified]
        FROM mm_bookmarks
        WHERE
            [identifier] == (SELECT [container] FROM mm_bookmarks WHERE [identifier] == (:container))
    UNION
        SELECT
            '0' AS [identifier],
            NULL AS [container],
            'CONTAINER' AS [type],
            NULL AS [url],
            '../' AS [title],
            NULL AS [note],
            NULL AS [created],
            NULL AS [modified]
        FROM mm_bookmarks
        WHERE
            [identifier] == (:container) AND [container] == '0'
    UNION
        SELECT
            [identifier],
            [container],
            [type],
            [url],
            [title],
            [note],
            [created],
            [modified]
        FROM mm_bookmarks WHERE [container] = (:container)
    ) ORDER BY CASE WHEN [title] == '../' THEN 0 ELSE 1 END;
    )EOF");
    query.bindValue(":container", _query);
    queryDb(this, query, mData, mCutModel);

    beginInsertRows(QModelIndex(), 0, mData.size() - 1);
    endInsertRows();

    clearSelections();
}

bool BookmarkListModel::insertBookmarks(QList<QVariantMap> const& _data)
{
    if (!mDb.isOpen())
        return false;
    if (_data.isEmpty())
        return false;

    QList<Bookmark*> data;
    convert(this, data, _data);

    QString sql = "INSERT INTO mm_bookmarks ([container], [type], [url], [title], [note]) VALUES ";
    QMap<QString, QString> bindings {};

    for (size_t i = 0; i < data.size(); ++i)
    {
        auto bm = data.at(i);

        if (bm->container.isEmpty())
            bm->container = QString::fromStdString(mm::bookmarks::sql::bookmarks::helpers::defaults::container);

        if (bm->type.toStdString() != mm::bookmarks::sql::bookmarks::helpers::type::container &&
            bm->type.toStdString() != mm::bookmarks::sql::bookmarks::helpers::type::url)
            return false;

        QString _sql {};

        QString _co = ":container" + QString::number(i);
        QString _ty = ":type" + QString::number(i);
        QString _ur = ":url" + QString::number(i);
        QString _ti = ":title" + QString::number(i);
        QString _no = ":note" + QString::number(i);

        _sql += "(" + _co + ", " + _ty + ", " + _ur + ", " + _ti + ", " + _no + ")";
        _sql += ((i + 1) < data.size()) ? ", " : "";
        sql += _sql;

        bindings[_co] = bm->container;
        bindings[_ty] = bm->type;
        bindings[_ur] = (!bm->url.isEmpty()) ? bm->url : nullptr;
        bindings[_ti] = (!bm->title.isEmpty()) ? bm->title : nullptr;
        bindings[_no] = (!bm->note.isEmpty()) ? bm->note : nullptr;
    }

    sql += ";";

    QSqlQuery query(mDb);
    query.prepare(sql);

    for (auto [key, value] : bindings.asKeyValueRange())
        query.bindValue(key, value);

    if (!query.exec())
    {
        qDebug() << query.lastError() << query.executedQuery() << bindings;
        query.finish();
        return false;
    }

    query.finish();
    return true;
}

//void BookmarkListModel::test(QVariantList o)
//{
//    qDebug() << __LINE__ << o;
//}
//void BookmarkListModel::test(QVariantMap o)
//{
//    qDebug() << __LINE__ << o;
//}
//void BookmarkListModel::test(QList<QVariantMap>const& o)
//{
//    qDebug() << __LINE__ << o;
//    for (auto v : o)
//    {
//        for (auto [key, value] : v.asKeyValueRange())
//        {
//            qDebug() << __LINE__ << key << value.toString();
//        }
//    }
//}
//void BookmarkListModel::test(Bookmark o)
//{
//    qDebug() << __LINE__ << o;
//}

bool BookmarkListModel::updateBookmarks(QList<QVariantMap> const& _data)
{
//    qDebug() << __LINE__ << _data[0].keys().isEmpty();
    if (!mDb.isOpen())
        return false;
    if (_data.isEmpty())
        return false;

    QList<Bookmark*> data;
    convert(this, data, _data);

    for (size_t i = 0; i < data.size(); ++i)
    {
        auto bm = data.at(i);

        if (bm->identifier.isEmpty())
            continue;

        QList<QString> items;
        QList<QPair<QString, QString>> binds;

        QString _id = ":identifier" + QString::number(i);
        QString _co = ":container" + QString::number(i);
        QString _ur = ":url" + QString::number(i);
        QString _ti = ":title" + QString::number(i);
        QString _no = ":note" + QString::number(i);

        binds.append({_id, bm->identifier});

        if (!bm->container.isEmpty())
        {
            items.append("[container] = " + _co);
            binds.append({_co, bm->container});
        }
        if (!bm->url.isEmpty())
        {
            items.append("[url] = " + _ur);
            binds.append({_ur, bm->url});
        }
        if (!bm->title.isEmpty())
        {
            items.append("[title] = " + _ti);
            binds.append({_ti, bm->title});
        }
        if (!bm->note.isEmpty())
        {
            items.append("[note] = " + _no);
            binds.append({_no, bm->note});
        }

        QString sql = "UPDATE mm_bookmarks SET";

        for (int i = 0; i < items.size(); ++i)
            sql += (i <= 0 ? " " : ", ") + items.at(i);

        // sql += " [container] = :container";
        // sql += ", [url] = :url";
        // sql += ", [title] = :title";
        // sql += ", [note] = :note";

        sql += " WHERE [identifier] == " + _id + ";";
//        qDebug() << __LINE__ << sql;
        QSqlQuery query(mDb);
        query.prepare(sql);
//        qDebug() << __LINE__ << query.lastError();

        for (auto& i : binds)
            query.bindValue(i.first, i.second);

//        qDebug() << __LINE__ << items << binds;

        // query.bindValue(":container", bm->container);
        // query.bindValue(":url", bm->url);
        // query.bindValue(":title", bm->title);
        // query.bindValue(":note", bm->note);
        // query.bindValue(":identifier", bm->identifier);

        if (!query.exec())
        {
            qDebug() << query.lastError() << query.executedQuery() << binds;
            query.finish();
            return false;
        }

//        qDebug() << "Query:" << query.executedQuery();
//        qDebug() << "Bounds:" << query.boundValues();

        query.finish();

        if ((i + 1) == data.size())
            return true;
    }

    return false;
}

bool BookmarkListModel::deleteBookmarks(QList<QString> const& data)
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

    QSqlQuery query(mDb);
    query.prepare(sql);

    for (auto [key, value] : bindings.asKeyValueRange())
        query.bindValue(key, value);

    if (!query.exec())
    {
        qDebug() << query.lastError() << query.executedQuery() << bindings;
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
            QSqlQuery query(db);
            query.prepare(v);
            if (!query.exec())
                qDebug() << query.lastError() << query.executedQuery();
            query.finish();
        }
        // detach
        QSqlQuery query(db);
        query.prepare(dt);
        if (!query.exec())
            qDebug() << query.lastError() << query.executedQuery();
        query.finish();
    };

    static auto _import_prepare_and_process = [](QSqlDatabase& db, QString const& att, QString const& path_,
                                                 QList<QString> const& prep, QList<QString> const& proc)
    {
        // prepare
        QString _att = att.arg(path_);
//        _att.replace("'", "\\'");
//        _att.replace("\"", "\\\"");
//        _att.replace(";", "\\;");

        QSqlQuery query(db);
        query.prepare(_att);
        if (!query.exec())
            qDebug() << query.lastError() << query.executedQuery();
        qDebug() << query.lastError() << query.executedQuery();
        query.finish();

        for (auto const& v : prep)
        {
            QSqlQuery query(db);
            query.prepare(v);
            if (!query.exec())
                qDebug() << query.lastError() << query.executedQuery();
            qDebug() << query.lastError() << query.executedQuery();
            query.finish();
        }

        // process
        for (auto const& v : proc)
        {
            QSqlQuery query(db);
            query.prepare(v);
            if (!query.exec())
                qDebug() << query.lastError() << query.executedQuery();
            qDebug() << query.lastError() << query.executedQuery();
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
    QSqlQuery query(mDb);
    query.prepare("VACUUM;");
    return query.exec();
}

void BookmarkListModel::convert(QObject* parent, QList<Bookmark*>& result, QList<QVariantMap> const& other) const
{
//    qDebug() << __LINE__;
    for (auto v : other)
    {
//        qDebug() << __LINE__ << v.keys().isEmpty();
        Bookmark* bm = new Bookmark(parent);
        for (auto [key, value] : v.asKeyValueRange())
        {
//            qDebug() << __LINE__ << key << value;
            QString _v = value.toString();
            if (key == "identifier")
                bm->identifier = _v;
            else if (key == "container")
                bm->container = _v;
            else if (key == "type")
                bm->type = _v;
            else if (key == "url")
                bm->url = _v;
            else if (key == "title")
                bm->title = _v;
            else if (key == "note")
                bm->note = _v;
            else if (key == "created")
                bm->created = _v;
            else if (key == "modified")
                bm->modified = _v;
        }
        result.append(bm);
    }
//    qDebug() << __LINE__;
}


void BookmarkListModel::goInto(QString const& parent)
{
    if (parent.isEmpty())
        return;
    selectFromContainerIntoModel(parent);
    //mParentsHistory.append(parent);
}

//void BookmarkListModel::goBack()
//{
//    if (!goBackable())
//        return;
//    mParentsHistory.takeLast();
//    if (goBackable())
//        goInto(mParentsHistory.takeLast());
//    else
//        goHome();
//}

//bool BookmarkListModel::goBackable()
//{
//    qDebug() << "goBackable" << QTime::currentTime() << mParentsHistory << (mParentsHistory.size() > 1);
//    return mParentsHistory.size() > 1;
//}

void BookmarkListModel::goHome()
{
    goInto("0");
}

void BookmarkListModel::goRefresh()
{
    goInto(currentContainer());
//    if (mParentsHistory.isEmpty())
//        return;
//    goInto(mParentsHistory.takeLast());
}

QString BookmarkListModel::currentContainer()
{
    return (mCurrentContainer.isEmpty() || mCurrentContainer == "") ? "0" : mCurrentContainer;
//    if (mParentsHistory.isEmpty())
//        return "";
//    return mParentsHistory.last();
}

void BookmarkListModel::selectToggle(int const& _index)
{
    QModelIndex idx = index(_index, 0);
    if (!idx.isValid())
        return;
    mSelModel.select(idx, QItemSelectionModel::Toggle);
    mData.at(idx.row())->selected = mSelModel.isSelected(idx);
    emit dataChanged(idx, idx);
    emit selectionsSizeChanged();
}

bool BookmarkListModel::selectSelected(int const& _index)
{
    return mSelModel.isSelected(index(_index, 0));
}

bool BookmarkListModel::selectHasSelection()
{
    return mSelModel.hasSelection();
}

QList<QVariantMap> BookmarkListModel::selectGetSelections()
{
    QList<QVariantMap> res;
    for (auto v : mSelModel.selectedIndexes())
    {
        if (!v.isValid())
            continue;
        res.append(mData.at(v.row())->asVariantMap());
    }
    return res;
}

void BookmarkListModel::clearSelections()
{
    for (auto v : mSelModel.selectedIndexes())
    {
        selectToggle(v.row());
    }
}

void BookmarkListModel::cutSelections()
{
    mCutModel.clear();

    for (auto v : mSelModel.selectedIndexes())
    {
        if (!v.isValid())
            continue;
        mData.at(v.row())->cut = true;
        mCutModel.append(mData.at(v.row())->asVariantMap());
        emit dataChanged(v, v);
    }

    emit cutSizeChanged();

    clearSelections();
}

bool BookmarkListModel::cutHasSelection()
{
    return mCutModel.size() > 0;
}

bool BookmarkListModel::cutPaste()
{
    if (mCutModel.isEmpty())
        return false;
    if (mCurrentContainer.isEmpty())
        return false;

    QList<QVariantMap> bms;

    for (auto v : mCutModel)
    {
        QVariantMap bm;
        bm["identifier"] = v["identifier"];
        bm["container"] = mCurrentContainer;
        bms.append(bm);
    }

    bool res = updateBookmarks(bms);

    mCutModel.clear();
    emit cutSizeChanged();

    return res;
}

QVariantMap BookmarkListModel::getTypesCount()
{
    auto _queryDb = [](QSqlQuery& _query) -> int
    {
        if (!_query.exec())
        {
            qDebug() << _query.lastError() << _query.executedQuery();
            _query.finish();
            return 0;
        }
        int colCount = _query.record().indexOf("COUNT([type])");
        QString count;
        while (_query.next())
            count = _query.value(colCount).toString();
        _query.finish();
        return count.toInt();
    };

    QVariantMap res;
    QSqlQuery query(mDb);

    query.prepare("SELECT COUNT([type]) FROM mm_bookmarks WHERE [type] == 'CONTAINER'");
    res["CONTAINER"] = _queryDb(query);

    query.prepare("SELECT COUNT([type]) FROM mm_bookmarks WHERE [type] == 'URL'");
    res["URL"] = _queryDb(query);

    return res;
}

QString BookmarkListModel::getDatabasePath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/mm_bookmarks.db";
    qDebug() << "Database:" << path;
    return path;
//    QSettings set(settingsOrg, settingsApp);
//    qDebug() << "Settings File:" << set.fileName();
//    QVariant val = set.value(settingsDir, QVariant{});
//    if (val.isNull() || !val.isValid())
//    {
//        qDebug() << "Database Directory: <invalid>";
//        return QString();
//    }
//    QString path = val.toString();
//    if (path.isEmpty())
//    {
//        qDebug() << "Database Directory: <empty>";
//        return QString();
//    }
//    qDebug() << "Database Directory:" << path;
//    return path;
}

//bool BookmarkListModel::setDatabaseDirectory(QString const& path)
//{
//    if (path.isEmpty())
//    {
//        qDebug() << "Database Directory Set: <empty>" << path;
//        return false;
//    }
//    QSettings set(settingsOrg, settingsApp);
//    qDebug() << "Settings File:" << set.fileName();
//    set.setValue(settingsDir, path);
//    set.sync();
//    qDebug() << "Database Directory Set:" << path;
//    return true;
//}

QString BookmarkListModel::toLocalFile(QString const& path) const
{
    return QUrl(path).toLocalFile();
}
