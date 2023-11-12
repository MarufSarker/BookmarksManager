#ifndef UTILS_HH
#define UTILS_HH

#ifdef __ANDROID__
#include <jni.h>
#endif

bool checkAndroidPermissions();

void registerJNIMethods();

#ifdef __ANDROID__
static jboolean jniInsertBookmark(JNIEnv *env, jobject thiz, jstring title, jstring url);
#endif

#endif // UTILS_HH
