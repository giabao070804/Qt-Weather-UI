cmake_minimum_required(VERSION 3.19)
project(weather LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()
qt_add_resources(WEATHER_RESOURCES Resources.qrc)
qt_add_executable(weather
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${WEATHER_RESOURCES}

)

target_link_libraries(weather
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS weather
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET weather
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
