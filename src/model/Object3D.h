#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Coordinate.h"
#include "GraphicObject.h"

namespace model {
    class Object3D : public utils::Clonable<GraphicObject, Object3D> {
    public:
        Object3D(const std::string &name, const std::vector<Coordinate<3>> &coordinates);

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

