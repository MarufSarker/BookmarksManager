#include "utils.hh"

#include <QString>
#include <QCoreApplication>
#ifdef __ANDROID__
#include <jni.h>
#include <QtCore/private/qandroidextras_p.h>
#endif
#include "bookmarklistmodel.h"

bool checkAndroidPermissions()
{
#ifdef __ANDROID__
    QString perm = (QNativeInterface::QAndroidApplication::sdkVersion() >= 30)
                       ? "android.settings.MANAGE_ALL_FILES_ACCESS_PERMISSION"
                       : "android.permission.WRITE_EXTERNAL_STORAGE";

    auto res = QtAndroidPrivate::checkPermission(perm).result();

    if (res == QtAndroidPrivate::Authorized)
        return true;

    res = QtAndroidPrivate::requestPermission(perm).result();

    return (res == QtAndroidPrivate::Authorized);
#else
    return false;
#endif
}

void registerJNIMethods()
{
#ifdef __ANDROID__
    const JNINativeMethod methods[] = {
        {
            "callInsertBookmark",
            "(Ljava/lang/String;Ljava/lang/String;)Z",
            reinterpret_cast<void *>(jniInsertBookmark)
        }
    };

    QJniEnvironment env;

    env.registerNativeMethods(
        "mm/android/bookmarksmanager/BookmarkAdd",
        methods,
        sizeof(methods) / sizeof(methods[0])
    );
#endif
}

#ifdef __ANDROID__
jboolean jniInsertBookmark(JNIEnv *env, jobject thiz, jstring _title, jstring _url)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    QString title = QJniObject(_title).toString();
    QString url = QJniObject(_url).toString();

    BookmarkListModel model {nullptr, "androidDb"};

    QVariantMap bm;
    bm["url"] = url;
    bm["title"] = title;
    bm["type"] = "URL";
    bm["container"] = model.currentContainer();

    QList<QVariantMap> bms;
    bms.append(bm);

    return model.insertBookmarks(bms);
}
#endif
