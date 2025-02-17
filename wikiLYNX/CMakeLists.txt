cmake_minimum_required(VERSION 3.16)

project(wikilynx VERSION 1.5.6 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebEngineWidgets)

if (MSVC)
    #find_package(jsoncpp CONFIG REQUIRED)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/assets/logo.rc")
    #qt_add_executable(photosurfaceexample main.cpp ${app_icon_resource_windows})
endif (MSVC)

set(PROJECT_SOURCES
        main.cpp
        gamewindow.cpp
        include/gamewindow.h
        ui/gamewindow.ui
        assets/assets.qrc

        ui/welcome.ui include/welcome.h welcome.cpp
        ui/congrats.ui congrats.cpp include/congrats.h
        ui/about.ui about.cpp include/about.h
        ui/basebrowser.ui basebrowser.cpp include/basebrowser.h
        ui/terms.ui terms.cpp include/terms.h
        ui/levelmanager.ui levelmanager.cpp include/levelmanager.h
        ui/leveleditor.ui leveleditor.cpp include/leveleditor.h
        ui/leaderboard.ui leaderboard.cpp include/leaderboard.h
        include/getlevel.h getlevel.cpp ui/getlevel.ui
        include/loadingscreen.h loadingscreen.cpp ui/loadingscreen.ui
        include/news.h news.cpp ui/news.ui
        include/statusoverview.h statusoverview.cpp ui/statusoverview.ui
        include/levels.h levels.cpp ui/levels.ui
        include/whatsnew.h whatsnew.cpp ui/whatsnew.ui
        include/baselist.h baselist.cpp ui/baselist.ui
        include/scoresheet.h scoresheet.cpp
        renovatio.cpp
        include/renovatio.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wikilynx
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET wikilynx APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(wikilynx SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(wikilynx
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_link_libraries(wikilynx PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebEngineWidgets Qt${QT_VERSION_MAJOR}::Multimedia)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER in.org.dawn.wikilynx)
endif()
set_target_properties(wikilynx PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
include_directories("ui include")

install(TARGETS wikilynx
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(UNIX AND NOT APPLE)
    # Linux Desktop Files
    install (FILES ../docs/desktop/in.org.dawn.wikilynx.svg DESTINATION share/icons/hicolor/scalable/apps)
    install (FILES ../docs/desktop/in.org.dawn.wikilynx.appdata.xml DESTINATION share/metainfo)
    install (FILES ../docs/desktop/in.org.dawn.wikilynx.desktop DESTINATION share/applications)

    # License
    install (FILES ../LICENSE DESTINATION share/licenses/wikilynx)
    install (FILES ../COPYING DESTINATION share/licenses/wikilynx)
    install (FILES ../COPYING.QtWebEngine DESTINATION share/licenses/wikilynx)
    install (FILES ../docs/license/Comfortaa_OFL.txt DESTINATION share/licenses/wikilynx)
    install (FILES ../docs/license/CourierPrime_OFL.txt DESTINATION share/licenses/wikilynx)
    install (FILES ../docs/license/NotoSans_OFL.txt DESTINATION share/licenses/wikilynx)
    install (FILES ../docs/license/LICENSE-MaterialIcons.txt DESTINATION share/licenses/wikilynx)
endif()


if(WIN32)
    # License
    install (FILES ../LICENSE DESTINATION docs/licenses)
    install (FILES ../COPYING DESTINATION docs/licenses)
    install (FILES ../COPYING.QtWebEngine DESTINATION docs/licenses)
    install (FILES ../docs/license/Comfortaa_OFL.txt DESTINATION docs/licenses)
    install (FILES ../docs/license/CourierPrime_OFL.txt DESTINATION docs/licenses)
    install (FILES ../docs/license/NotoSans_OFL.txt DESTINATION docs/licenses)
    install (FILES ../docs/license/LICENSE-MaterialIcons.txt DESTINATION docs/licenses)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wikilynx)
endif()


