cmake_minimum_required(VERSION 2.8)
project(utils)

set(SOURCES
    Clonable.h
    csv.h
    Matrix.h
    make_unique.h
    split.h
    tiny_obj_loader.h
    tiny_obj_loader.cpp
)


ADD_LIBRARY_CG(utils ${SOURCES})