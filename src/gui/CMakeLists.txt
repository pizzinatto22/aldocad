cmake_minimum_required(VERSION 2.8)
project(gui)

qt5_wrap_ui(UI_HEADERS
    ui/BSplineDialog.ui
    ui/LineDialog.ui
    ui/MainView.ui
    ui/Object3DDialog.ui
    ui/PointDialog.ui
    ui/PolygonDialog.ui
    ui/Rotate3DDialog.ui
    ui/RotateDialog.ui
    ui/ScaleDialog.ui
    ui/SurfaceDialog.ui
    ui/TranslateDialog.ui
)

set(SOURCES
	BSplineDialog.cpp
	BSplineDialog.h
    LineDialog.cpp
    LineDialog.h
    MainView.cpp
    MainView.h
    Object3DDialog.cpp
    Object3DDialog.h
    PointDialog.cpp
    PointDialog.h
    PolygonDialog.cpp
    PolygonDialog.h
    Rotate3DDialog.cpp
    Rotate3DDialog.h
    RotateDialog.cpp
    RotateDialog.h
    ScaleDialog.cpp
    ScaleDialog.h
    SurfaceDialog.cpp
    SurfaceDialog.h
    TranslateDialog.cpp
    TranslateDialog.h
)

include_directories(
	${utils_SOURCE_DIR}
    ${model_SOURCE_DIR}
	${controller_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Qt5Core_INCLUDE_DIRS}
)

ADD_LIBRARY_CG(gui ${UI_HEADERS} ${SOURCES})

target_link_libraries(gui
	Qt5::Core
	Qt5::Gui
	model
)
