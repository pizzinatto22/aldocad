#include "Object3D.h"

namespace model {
    Object3D::Object3D(const std::string &name, const std::vector<Coordinate<3>> &coordinates):
        ClonableBase{name},
        coordinates_{coordinates}
    {}

    const std::vector<Coordinate<3>> &Object3D::coordinates()
    {
        return coordinates_;
    }

    const std::vector<Coordinate<3>> &Object3D::originalCoordinates()
    {
    	return coordinates();
    }


    GraphicType Object3D:: type(){
    	return OBJECT3D;
    }

    bool Object3D::isEmpty() const
    {
    	return coordinates_.size() == 0;
    }

    void Object3D::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    }
}

