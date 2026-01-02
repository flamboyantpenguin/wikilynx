cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Multimedia WebEngineWidgets Network)

if (MSVC)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/assets/logo.rc")
endif (MSVC)

set(PROJECT_SOURCES
    main.cpp
    assets/assets.qrc
)

add_subdirectory(ui)
add_subdirectory(game)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wikilynx
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )
else()
    if(ANDROID)
        add_library(wikilynx SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(wikilynx
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(wikilynx PRIVATE Qt${QT_VERSION_MAJOR}::Widgets wikilynx_game wikilynx_ui)

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

install(TARGETS wikilynx
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

