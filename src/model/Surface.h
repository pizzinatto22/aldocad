#ifndef SURFACE_H
#define SURFACE_H

#include "Coordinate.h"
#include "GraphicObject.h"

namespace model {
    class Surface : public utils::Clonable<GraphicObject, Surface> {
    public:
        Surface(const std::string &name, const unsigned lines, const std::vector<Coordinate<3>> &coordinates);
        Surface(const std::string &name, const unsigned lines, const std::vector<Coordinate<3>> &coordinates, const std::vector<Coordinate<3>> &points);

        const std::vector<Coordinate<3>> &coordinates() override;
        const std::vector<Coordinate<3>> &originalCoordinates() override;
        GraphicType type() override;
        bool isEmpty() const override;
        void changeCoordinates(std::vector<Coordinate<3>> newCoordinates) override;

        const unsigned lines() const;

    private:

        unsigned lines_;
        std::vector<Coordinate<3>> coordinates_;
        std::vector<Coordinate<3>> points_;

        void generatePoints(Coordinate<3> p0, Coordinate<3> p1, Coordinate<3> p2, Coordinate<3> p3);

        void forwardDifference(unsigned n,
							   double x, double dx, double d2x, double d3x,
							   double y, double dy, double d2y, double d3y,
							   double z, double dz, double d2z, double d3z);
    };

}

#endif

