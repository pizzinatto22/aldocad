#include "Point.h"

namespace model {
    Point::Point(const std::string &name, double x, double y):
        ClonableBase{name},
        coordinates_{{Coordinate<3>(x, y, 0.0)}}
    {}

    Point::Point(const std::string &name):
    		ClonableBase{name},
    		coordinates_{{}}
    {}

    const std::vector<Coordinate<3>> &Point::coordinates()
    {
    	return coordinates_;
    }

    const std::vector<Coordinate<3>> &Point::originalCoordinates()
    {
    	return coordinates();
    }


    GraphicType Point::type()
    {
    	return POINT;
    }

    bool Point::isEmpty() const
    {
    	return coordinates_.size() == 0;
    }

    void Point::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    }
}
