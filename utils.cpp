#include "utils.h"

#include <algorithm>

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
    QList<QString> permissions ({
        (QNativeInterface::QAndroidApplication::sdkVersion() >= 30)
        ? "android.settings.MANAGE_ALL_FILES_ACCESS_PERMISSION"
        : "android.permission.WRITE_EXTERNAL_STORAGE",
    });
    QList<bool> results;

    for (auto const& permission : permissions)
    {
        auto res = QtAndroidPrivate::checkPermission(permission).result();
        bool authoried = (res == QtAndroidPrivate::Authorized);
        results.append(authoried);
        qDebug() << "ANDROID PERMISSION:" << permission << "AUTHORIZED:" << authoried;
    }

    return std::all_of(results.constBegin(), results.constEnd(), [](bool v) { return v; });
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
    bm["container"] = model.getCurrentContainer();

    QList<QVariantMap> bms;
    bms.append(bm);

    return model.insertBookmarks(bms);
}
#endif
