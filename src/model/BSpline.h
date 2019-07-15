#ifndef BSPLINE_H
#define BSPLINE_H

#include "Coordinate.h"
#include "GraphicObject.h"

namespace model {
    class BSpline : public utils::Clonable<GraphicObject, BSpline> {
    public:
        BSpline(const std::string &name, const std::vector<Coordinate<3>> &coordinates);
        BSpline(const std::string &name, const std::vector<Coordinate<3>> &coordinates,const std::vector<Coordinate<3>> &points);

        const std::vector<Coordinate<3>> &coordinates() override;
        const std::vector<Coordinate<3>> &originalCoordinates() override;
        GraphicType type() override;
        bool isEmpty() const override;
        void changeCoordinates(std::vector<Coordinate<3>> newCoordinates) override;

    private:
        std::vector<Coordinate<3>> coordinates_;
        std::vector<Coordinate<3>> points_;
        void forwardDifference(unsigned n,
							   double x, double dx, double d2x, double d3x,
							   double y, double dy, double d2y, double d3y,
							   double z, double dz, double d2z, double d3z);
    };

}

#endif

