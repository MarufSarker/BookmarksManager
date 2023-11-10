#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QAbstractListModel>
#include <QQmlContext>
#include <QDirIterator>
#include <QtQml/qqml.h>
#include <QCommandLineParser>
#include "bookmark.h"
#include "bookmarklistmodel.h"

#ifdef __ANDROID__
#include <jni.h>
#include <QJniEnvironment>
#include <QJniObject>
#include <QCoreApplication>
#include <QtCore/private/qandroidextras_p.h>
#endif

#ifdef __ANDROID__
static int fromJavaOne(JNIEnv *env, jobject thiz, jint x)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    qDebug() << x << "< 100";
    return x + 1;
}

bool checkPermission()
{
    QString perm = (QNativeInterface::QAndroidApplication::sdkVersion() >= 30)
                       ? "android.settings.MANAGE_ALL_FILES_ACCESS_PERMISSION"
                       : "android.permission.WRITE_EXTERNAL_STORAGE";

    auto res = QtAndroidPrivate::checkPermission(perm).result();

    if (res == QtAndroidPrivate::Authorized)
        return true;

    res = QtAndroidPrivate::requestPermission(perm).result();

    return (res == QtAndroidPrivate::Authorized);


//    auto r = QtAndroidPrivate::checkPermission(QtAndroidPrivate::Storage).result();
//    if (r == QtAndroidPrivate::Denied)
//    {
//        r = QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage).result();
//        if (r == QtAndroidPrivate::Denied)
//            return false;
//    }
//    return true;
}
#endif

//BookmarkListModel model {};

#ifdef __ANDROID__
//QString toQString(JNIEnv *env, jstring str)
//{
//    if (!env)
//        return QString();
//    if (str == 0)
//        return QString();
//    int length = env->GetStringLength(str);
//    if (length == 0)
//        return QString();
//    jchar const* strPtr = env->GetStringChars(str, 0);
//    if (strPtr == 0)
//        return QString();
//    QString res = QString(reinterpret_cast<QChar const*>(strPtr), length);
//    env->ReleaseStringChars(str, strPtr);
//    return res;
//}

static jboolean jniInsertBookmark(JNIEnv *env, jobject thiz, jstring title, jstring url)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    QJniObject __title (title);
    QString _title = __title.toString();
    QJniObject __url (url);
    QString _url = __url.toString();

    BookmarkListModel model {nullptr, "androidDb"};

    QVariantMap bm;
    bm["url"] = _url;
    bm["title"] = _title;
    bm["type"] = "URL";
    bm["container"] = model.currentContainer();

    QList<QVariantMap> bms;
    bms.append(bm);

    return model.insertBookmarks(bms);
}
#endif

int main(int argc, char *argv[])
{
    for (auto v : QList({":", "qrc:", "assets:"}))
    {
        QDirIterator it(v, QDirIterator::Subdirectories);
        while (it.hasNext())
            qDebug() << it.next();
    }

//    QDirIterator it(":", QDirIterator::Subdirectories);
//    while (it.hasNext())
//        qDebug() << it.next();

    QGuiApplication app(argc, argv);

//    QStringList _args = QCoreApplication::arguments();
//    for (auto& v : _args)
//    {
//        qDebug() << v;
//    }
//    qDebug() << _args;
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
    // register the native methods first, ideally it better be done with the app start
    const JNINativeMethod methods[] = {
//        {"callNativeOne", "(I)I", reinterpret_cast<int *>(fromJavaOne)},
        {"callInsertBookmark", "(Ljava/lang/String;Ljava/lang/String;)Z", reinterpret_cast<void *>(jniInsertBookmark)}
    };
    QJniEnvironment env;
    env.registerNativeMethods("mm/android/bookmarksmanager/BookmarkAdd", methods, sizeof(methods) / sizeof(methods[0]));

//    bool permGranted = QJniObject::callStaticMethod<jboolean>(
//        "org/qtproject/example/BookmarkAdd",
//        "permissionGranted",
//        "(Landroid/content/Context;)Z",
//        QNativeInterface::QAndroidApplication::context());

//    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]() {
//        QJniObject activity = QNativeInterface::QAndroidApplication::context();
//        // Hide system ui elements or go full screen
//        activity.callObjectMethod("processPermission", "()Landroid/view/Window;")
//            .callObjectMethod("getDecorView", "()Landroid/view/View;")
//            .callMethod<void>("setSystemUiVisibility", "(I)V", 0xffffffff);
//    }).waitForFinished();

    checkPermission();
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
//     qmlRegisterType<Bookmark>("Bookmark", 1, 0, "Bookmark");
    engine.rootContext()->setContextProperty("bookmarkListModel", &model);
    // engine.loadFromModule("BookmarksManager", "Main");
    engine.rootContext()->setContextProperty("listModel", &model);
    engine.loadFromModule("BookmarksManager", "Test");

    // TODO : REMOVE
//    model.search("0", "container", "");
    //model.selectFromContainerIntoModel("0");

    return app.exec();
}
