#ifndef LINE_H
#define LINE_H

#include "Coordinate.h"
#include "GraphicObject.h"

#include <vector>

namespace model {
    class Line : public utils::Clonable<GraphicObject, Line> {
    public:
        Line(const std::string &name,
                double x1,
                double y1,
                double x2,
                double y2);

        Line (const std:: string &name);

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
