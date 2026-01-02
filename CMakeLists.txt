cmake_minimum_required(VERSION 3.19)

project(wikilynx VERSION 1.5.8 LANGUAGES CXX)

add_subdirectory(src)

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
