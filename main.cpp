#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDirIterator>
#include <QCommandLineParser>

#include "bookmarklistmodel.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    for (auto v : QList({":", "qrc:", "assets:"}))
    {
        QDirIterator it(v, QDirIterator::Subdirectories);
        while (it.hasNext())
            qDebug() << it.next();
    }

    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({{"i", "import"}, "Import of <type>. [FIREFOX_SQLITE, MMBOOKMARKS]", "type"});
    parser.addOption({{"f", "file"}, "Import from <file>.", "file"});
    parser.process(app);

    bool cliImport = false;
    QString importType;
    QString importFile;

    if (parser.isSet("import"))
    {
        importType = parser.value("import");
        if (importType != "FIREFOX_SQLITE" && importType != "MMBOOKMARKS")
        {
            qDebug() << "Invalid Import Type";
            return 1;
        }
        if (!parser.isSet("file"))
        {
            qDebug() << "Must provide import file";
            return 1;
        }
        importFile = parser.value("file");
        cliImport = true;
    }

#ifdef __ANDROID__
    if (!checkAndroidPermissions())
        qDebug() << "Not All Android Permissions are Authorized";
    registerJNIMethods();
#endif

    BookmarkListModel model {nullptr, "qmlDb"};

    if (cliImport)
    {
        bool res = model.importFrom(importType, importFile);
        qDebug() << "CLI Import:" << (res ? "SUCCESS" : "FAIL");
        return res ? 0 : 1;
    }

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("bookmarkListModel", &model);
    engine.rootContext()->setContextProperty("listModel", &model);
    engine.loadFromModule("BookmarksManager", "Test");

    return app.exec();
}
