#include "Line.h"

namespace model {
    Line::Line(const std::string &name, double x1, double y1, double x2, double y2):
        ClonableBase{name},
        coordinates_{{Coordinate<3>(x1, y1, 0.0),Coordinate<3>(x2, y2, 0.0)}}
    {}

    Line::Line (const std:: string &name):
    	ClonableBase{name},
        coordinates_{{}}
    {}

    const std::vector<Coordinate<3>> &Line::coordinates()
    {
    	return coordinates_;
    }

    const std::vector<Coordinate<3>> &Line::originalCoordinates()
    {
    	return coordinates();
    }

    GraphicType Line::type()
    {
    	return LINE;
    }

    bool Line::isEmpty() const{
    	return coordinates_.size() == 0;
    }

    void Line::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    }
}

