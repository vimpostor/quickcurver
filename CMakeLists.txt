project(QuickCurver)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Core Svg Quick QuickControls2 REQUIRED)

set(SOURCES
	"src/main.cpp"
	"src/game.cpp"
	"src/curver.cpp"
	"src/segment.cpp"
	"src/gui.cpp"
	"src/models/playermodel.cpp"
	"src/explosion.cpp"
	"src/util.cpp"
	"src/headnode.cpp"
	"src/items/item.cpp"
	"src/items/speeditem.cpp"
	"src/itemfactory.cpp"
	"src/items/cleaninstallitem.cpp"
	"src/items/invisibleitem.cpp"
	"src/items/flashitem.cpp"
	"src/items/slowitem.cpp"
	"src/items/agileitem.cpp"
	"src/models/itemmodel.cpp"
	"src/qml/qml.qrc"
	"src/wall.cpp"
	"src/settings.cpp"
	"src/bot.cpp"
	"src/network/network.cpp"
	"src/network/client.cpp"
	"src/network/server.cpp"
	"src/models/chatmodel.cpp"
)

set(VENDOR_SOURCES
	"fluid/src/imports/core/iconsimageprovider.cpp"
	"fluid/src/imports/controls/iconthemeimageprovider.cpp"
	"fluid/icons/icons.qrc"
)

include_directories(
	"fluid/src/imports/core"
	"fluid/src/imports/controls"
	"src"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${VENDOR_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Svg Qt5::Quick Qt5::QuickControls2)
