set(SOURCES
    about.cpp include/about.h forms/about.ui
    basebrowser.cpp include/basebrowser.h forms/basebrowser.ui
    baselist.cpp include/baselist.h forms/baselist.ui
    congrats.cpp include/congrats.h forms/congrats.ui
    gamewindow.cpp include/gamewindow.h forms/gamewindow.ui
    getlevel.cpp include/getlevel.h forms/getlevel.ui
    leaderboard.cpp include/leaderboard.h forms/leaderboard.ui
    leveleditor.cpp include/leveleditor.h forms/leveleditor.ui
    levelmanager.cpp include/levelmanager.h forms/levelmanager.ui
    levels.cpp include/levels.h forms/levels.ui
    loadingscreen.cpp include/loadingscreen.h forms/loadingscreen.ui
    news.cpp include/news.h forms/news.ui
    statusoverview.cpp include/statusoverview.h forms/statusoverview.ui
    terms.cpp include/terms.h forms/terms.ui
    welcome.cpp include/welcome.h forms/welcome.ui
    whatsnew.cpp include/whatsnew.h forms/whatsnew.ui
)

#qt_wrap_ui(UI_WRAP ${UI_UIS}) # Only needed for older Qt versions
#qt5_wrap_cpp(UI_MOC ${UI_HEADERS}) # or qt_wrap_cpp for newer
set(CMAKE_AUTOUIC ON)
add_library(wikilynx_ui ${SOURCES} ${UI_MOC})
target_include_directories(wikilynx_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(wikilynx_ui PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::WebEngineWidgets Qt${QT_VERSION_MAJOR}::Multimedia wikilynx_game)
