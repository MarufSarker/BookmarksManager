cmake_minimum_required(VERSION 3.16)

project(BookmarksManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(BookmarksManager
    main.cpp
)

qt_add_qml_module(BookmarksManager
    URI BookmarksManager
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES bookmarklistmodel.h bookmarklistmodel.cpp
    SOURCES sql.hh
    SOURCES bookmark.h bookmark.cpp
    SOURCES utils.hh utils.cc
    QML_FILES Test.qml
)

set(M_BIG_RESOURCES "")

if(ANDROID)
    set(M_BIG_RESOURCES "BIG_RESOURCES")
endif()

qt_add_resources(BookmarksManager "assets" ${M_BIG_RESOURCES}
FILES
    assets/images/folder.svg
    assets/images/link.svg
)

set(QT_ANDROID_BUILD_ALL_ABIS TRUE CACHE BOOL "" FORCE)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(BookmarksManager PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BookmarksManager
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
)

if(NOT(ANDROID))
    set_target_properties(BookmarksManager PROPERTIES
        # as <build-dir>/Dictionary is an existing directory
        OUTPUT_NAME "${PROJECT_NAME}-${CMAKE_BUILD_TYPE}"
    )
endif()

target_link_libraries(BookmarksManager
    PRIVATE Qt6::Quick Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS BookmarksManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
