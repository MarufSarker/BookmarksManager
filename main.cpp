#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QAbstractListModel>
#include <QQmlContext>
#include <QDirIterator>
#include <QtQml/qqml.h>
#include "bookmark.h"
#include "bookmarklistmodel.h"

#ifdef __ANDROID__
#include <jni.h>
#include <QJniEnvironment>
#endif

#ifdef __ANDROID__
static int fromJavaOne(JNIEnv *env, jobject thiz, jint x)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    qDebug() << x << "< 100";
    return x + 1;
}
#endif

int main(int argc, char *argv[])
{
//    QDirIterator it(":", QDirIterator::Subdirectories);
//    while (it.hasNext())
//        qDebug() << it.next();


    QGuiApplication app(argc, argv);

#ifdef __ANDROID__
    // register the native methods first, ideally it better be done with the app start
    const JNINativeMethod methods[] =
        {{"callNativeOne", "(I)I", reinterpret_cast<int *>(fromJavaOne)}};
    QJniEnvironment env;
    env.registerNativeMethods("org/qtproject/example/BookmarkAdd", methods, 1);
#endif

    BookmarkListModel model {};

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
//     qmlRegisterType<Bookmark>("Bookmark", 1, 0, "Bookmark");
    engine.rootContext()->setContextProperty("bookmarkListModel", &model);
    engine.loadFromModule("BookmarksManager", "Main");

    // TODO : REMOVE
//    model.search("0", "container", "");
    model.selectFromContainerIntoModel("0");

    return app.exec();
}
