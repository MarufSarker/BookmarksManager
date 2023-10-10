cmake_minimum_required(VERSION 3.16)

project(BookmarksManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sql)


#if (ANDROID)
#    set(MM_SQLITEORG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sqlite-amalgamation-3370200" CACHE PATH "" FORCE)
#endif()
#set(MM_BOOKMARKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../mmbookmarks"
#    CACHE PATH "mmbookmarks sources directory")
#add_subdirectory("${MM_BOOKMARKS_DIR}" mmbookmarks)


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appBookmarksManager
    main.cpp
)

qt_add_qml_module(appBookmarksManager
    URI BookmarksManager
    VERSION 1.0
    QML_FILES Main.qml
    RESOURCES android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/java/org/qtproject/example/BookmarkAdd.java android/res/layout/bookmark_add.xml android/res/values/libs.xml android/AndroidManifest.xml android/build.gradle android/gradle.properties android/gradlew android/gradlew.bat
    SOURCES bookmarklistmodel.h bookmarklistmodel.cpp
    RESOURCES images/typeContainer.png images/typeURL.png
    SOURCES sql.hh
    SOURCES bookmark.h bookmark.cpp
    QML_FILES Test.qml
)

set(QT_ANDROID_BUILD_ALL_ABIS TRUE CACHE BOOL "" FORCE)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBookmarksManager PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBookmarksManager
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
    QT_ANDROID_BUILD_ALL_ABIS TRUE
)

target_link_libraries(appBookmarksManager
    PRIVATE Qt6::Quick Qt6::Sql # mmbookmarks
)

#target_include_directories(appBookmarksManager
#PUBLIC
#    "${MM_BOOKMARKS_DIR}/sources"
#)

include(GNUInstallDirs)
install(TARGETS appBookmarksManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
