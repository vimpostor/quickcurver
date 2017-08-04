project(QuickCurver)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Svg Quick QuickControls2 REQUIRED)

set(SOURCES
	"src/items/curveitem.cpp"
	"src/dieanimation.cpp"
	"src/items/fastitem.cpp"
	"src/game.cpp"
	"src/headnode.cpp"
	"src/main.cpp"
	"src/qcurver.cpp"
	"src/segment.cpp"
	"src/wallnode.cpp"
	"src/aicontroller.cpp"
	"src/items/cleaninstallitem.cpp"
	"src/items/fatteritem.cpp"
	"src/cleaninstallanimation.cpp"
	"src/network/server.cpp"
	"src/network/client.cpp"
	"src/network/network.cpp"
	"src/items/invisibleitem.cpp"
	"src/qml/qml.qrc"
	)

set(VENDOR_SOURCES
	"fluid/src/imports/core/iconsimageprovider.cpp"
	"fluid/src/imports/controls/iconthemeimageprovider.cpp"
	)

include_directories("fluid/src/imports/core"
	"fluid/src/imports/controls")

add_executable(${PROJECT_NAME} ${SOURCES} ${VENDOR_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Svg Qt5::Quick Qt5::QuickControls2)
