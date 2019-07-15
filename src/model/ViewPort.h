#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Coordinate.h"
#include "Matrix.h"
#include "Window.h"

#include <vector>
#include <string>

namespace model {
    class ViewPort {
    public:
        ViewPort(Window *window, double height,double offset);
        std::vector<std::unique_ptr<GraphicObject>> transformedObjects() const;
            
    private:
        Window *window_;
        double height_;
        double offset_;


    };
}

#endif

