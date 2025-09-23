set(SOURCES
    scoresheet.cpp include/scoresheet.h
    renovatio.cpp include/renovatio.h
    oscillator.cpp include/oscillator.h
    gameboi.cpp include/gameboi.h
)

#qt_wrap_ui(UI_WRAP ${UI_UIS}) # Only needed for older Qt versions
#qt5_wrap_cpp(UI_MOC ${UI_HEADERS}) # or qt_wrap_cpp for newer

set(CMAKE_AUTOUIC ON)
add_library(wikilynx_game ${SOURCES} ${UI_MOC})
target_include_directories(wikilynx_game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} include)
#solink_directories(include)
target_link_libraries(wikilynx_game PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia)
