cmake_minimum_required(VERSION 2.8)
project(app)

set(SOURCES
  main.cpp
)

include_directories(
	${utils_SOURCE_DIR}
	${utils_BINARY_DIR}
    ${Qt5Widgets_INCLUDE_DIRS}
	${model_SOURCE_DIR}
	${controller_SOURCE_DIR}
    ${gui_SOURCE_DIR}
    ${gui_BINARY_DIR}
)

ADD_EXECUTABLE_CG(app ${SOURCES})

target_link_libraries(app
	utils
	controller
    Qt5::Widgets
    gui 
)
