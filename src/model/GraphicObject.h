#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

#include "Clonable.h"
#include "Coordinate.h"
#include "Matrix.h"

#include <string>
#include <vector>

namespace model {

	enum GraphicType{
		LINE,
		POINT,
		POLYGON,
		BSPLINE,
		OBJECT3D,
		SURFACE,
	};

    class GraphicObject : public utils::Clonable<GraphicObject> {
        public:
            GraphicObject(const std::string &name);
            virtual ~GraphicObject(){};

            virtual const std::vector<Coordinate<3>> &coordinates() = 0;
            virtual const std::vector<Coordinate<3>> &originalCoordinates() = 0;

            virtual GraphicType type() = 0;
            virtual bool isEmpty() const = 0;
            virtual void changeCoordinates(std::vector<Coordinate<3>> newCoordinates) = 0;

            const std::string &name() const;

            void transform(utils::Matrix<4,4> transformation);
            Coordinate<3> center();

        private:
            std::string name_;
    };
}

#endif
