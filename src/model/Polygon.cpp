#include "Polygon.h"

namespace model {
    Polygon::Polygon(const std::string &name, const std::vector<Coordinate<3>> &coordinates):
        ClonableBase{name},
        coordinates_{coordinates}
    {}

    const std::vector<Coordinate<3>> &Polygon::coordinates()
    {
        return coordinates_;
    }

    const std::vector<Coordinate<3>> &Polygon::originalCoordinates()
    {
    	return coordinates();
    }


    GraphicType Polygon:: type(){
    	return POLYGON;
    }

    bool Polygon::isEmpty() const
    {
    	return coordinates_.size() == 0;
    }

    void Polygon::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    }
}

