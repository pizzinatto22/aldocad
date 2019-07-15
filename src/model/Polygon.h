#ifndef POLYGON_H
#define POLYGON_H

#include "Coordinate.h"
#include "GraphicObject.h"

namespace model {
    class Polygon : public utils::Clonable<GraphicObject, Polygon> {
    public:
        Polygon(const std::string &name, const std::vector<Coordinate<3>> &coordinates);

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

