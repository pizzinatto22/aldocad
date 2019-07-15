cmake_minimum_required(VERSION 2.8)
project(model)


set(SOURCES
	BSpline.cpp
	BSpline.h
    Coordinate.h
    GraphicObject.h
    GraphicObject.cpp
    Line.h
    Line.cpp
    Object3D.cpp
    Object3D.h
    Point.h
    Point.cpp
    Polygon.h
    Polygon.cpp
    Surface.cpp
    Surface.h
    ViewPort.h
    ViewPort.cpp
    Window.h
    Window.cpp
    World.cpp
    World.h
)

include_directories(
    ${utils_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)

ADD_LIBRARY_CG(model ${SOURCES})

