project(bgestures)
cmake_minimum_required(VERSION 3.2)
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(bGuestures_SRCS
    bturngesture.cpp
    bturngesturev2.cpp
    bpolygongesture.cpp
    bgesturewidget.cpp
    bcutgesture.cpp
    main.cpp
)

add_executable(bgestures ${bGuestures_SRCS})
target_link_libraries(bgestures ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})

