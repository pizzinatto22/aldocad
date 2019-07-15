cmake_minimum_required(VERSION 2.8)
project(controller)

set(SOURCES
	Facade.h
	Facade.cpp
)

include_directories(
	${utils_SOURCE_DIR}
    ${model_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
)

ADD_LIBRARY_CG(controller ${SOURCES})

target_link_libraries(controller
	utils
	Qt5::Core
	Qt5::Gui
)
