cmake_minimum_required(VERSION 3.1.0)

project(QuickCurver)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Core Svg Quick QuickControls2 REQUIRED)

file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HDRS "src/*.h")
file(GLOB_RECURSE RSRC "src/*.qrc")

file(GLOB FLUID "fluid/icons/icons.qrc")

include_directories(
	"src"
)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${RSRC} ${FLUID})


target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Svg Qt5::Quick Qt5::QuickControls2)
