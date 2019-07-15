#ifndef POINT_H
#define POINT_H

#include "Coordinate.h"
#include "GraphicObject.h"

#include <vector>

namespace model {
    class Point : public utils::Clonable<GraphicObject,Point> {
        public:
            Point(const std::string &name, double x, double y);
            Point(const std::string &name);

            const std::vector<Coordinate<3>> &coordinates() override;
            const std::vector<Coordinate<3>> &originalCoordinates() override;
            GraphicType type() override;
            bool isEmpty() const override;
            void changeCoordinates(std::vector<Coordinate<3>> newCoordinates) override;

        private:
            std::vector<Coordinate<3>> coordinates_;

    };
}

#endif
