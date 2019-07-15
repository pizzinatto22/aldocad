#include "GraphicObject.h"

#include <iostream>

namespace model {
    GraphicObject::GraphicObject(const std::string &name):
        name_{name}
    {}

    const std::string &GraphicObject::name() const
	{
    	return name_;
	}

    void GraphicObject::transform(utils::Matrix<4,4> transformation){

    	std::vector<Coordinate<3>> oldCoordinates = originalCoordinates();
		std::vector<Coordinate<3>> newCoordinates = {};

		for (auto c : oldCoordinates){
			utils::Matrix<1,4> m;
			m(0,0) = c.values()[0];
			m(0,1) = c.values()[1];
			m(0,2) = c.values()[2];
			m(0,3) = 1;

			m = m * transformation;
			c = Coordinate<3>(m(0,0),m(0,1),m(0,2));

			newCoordinates.push_back(c);
		}

		changeCoordinates(newCoordinates);
    }


    Coordinate<3> GraphicObject::center()
    {
    	std::vector<Coordinate<3>> coord = coordinates();

    	double sumX = 0;
    	double sumY = 0;
    	double sumZ = 0;

    	for (auto c:coord) {
    		sumX += c.values()[0];
    		sumY += c.values()[1];
    		sumZ += c.values()[2];
    	}

    	sumX /= coord.size();
    	sumY /= coord.size();
    	sumZ /= coord.size();

    	return Coordinate<3>(sumX,sumY,sumZ);
    }

}
