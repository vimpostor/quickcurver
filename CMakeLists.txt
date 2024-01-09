cmake_minimum_required(VERSION 3.21)
project(quickcurver VERSION 0.1 DESCRIPTION "Modern Qt/C++ implementation of Achtung die Kurve with online multiplayer")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND QT_MODULES Core Qml Quick QuickControls2 Svg)
find_package(Qt6 6.6 COMPONENTS ${QT_MODULES} REQUIRED)
qt_standard_project_setup()
set(QT_PREFIXED_MODULES ${QT_MODULES})
list(TRANSFORM QT_PREFIXED_MODULES PREPEND "Qt6::")

include_directories("src")
add_compile_definitions(QUICKCURVER_VERSION="${PROJECT_VERSION}")

file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HDRS "src/*.hpp")
file(GLOB_RECURSE QMLS RELATIVE "${CMAKE_SOURCE_DIR}" "src/qml/*.qml")

qt_add_executable(${PROJECT_NAME} ${SRCS} ${RESOURCES})

qt_add_qml_module(${PROJECT_NAME} URI "Backend" VERSION 1.0 NO_RESOURCE_TARGET_PATH QML_FILES ${QMLS} SOURCES "src/backend.cpp")

include(FetchContent)
FetchContent_Declare(quartz GIT_REPOSITORY https://github.com/vimpostor/quartz.git GIT_TAG c67c8dc15dcf25573d2b70378f4fd22a65593939)
FetchContent_MakeAvailable(quartz)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_PREFIXED_MODULES})
quartz_link(${PROJECT_NAME})

# install
install(TARGETS ${PROJECT_NAME} RUNTIME)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/doc/man/" TYPE MAN)
install(FILES "assets/${PROJECT_NAME}.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
# icon
install(FILES "assets/${PROJECT_NAME}.svg" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
list(APPEND ICON_SIZES 16 32 48 64 128 256 512)
foreach(ICON_SIZE IN LISTS ICON_SIZES)
	list(APPEND ICON_COMMANDS COMMAND "convert" "${CMAKE_SOURCE_DIR}/assets/${PROJECT_NAME}.svg" "-resize" "${ICON_SIZE}x${ICON_SIZE}" "${ICON_SIZE}.png")
	install(FILES "${CMAKE_BINARY_DIR}/${ICON_SIZE}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps" RENAME "${PROJECT_NAME}.png")
endforeach()
add_custom_target(linux-desktop-integration ${ICON_COMMANDS})
