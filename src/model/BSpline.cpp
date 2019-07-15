#include "BSpline.h"
#include "Matrix.h"

#include <iostream>

using namespace std;


namespace model {
	using namespace utils;

    BSpline::BSpline(const std::string &name, const std::vector<Coordinate<3>> &coordinates):
        ClonableBase{name},
        coordinates_{coordinates},
        points_{{}}
    {}

    BSpline::BSpline(const std::string &name, const std::vector<Coordinate<3>> &coordinates, const std::vector<Coordinate<3>> &points):
        ClonableBase{name},
        coordinates_{coordinates},
        points_{points}
    {}


    const std::vector<Coordinate<3>> &BSpline::coordinates()
    {

    	if (points_.size() == 0 && !isEmpty()){
    		int i = 0;
    		int segments = coordinates_.size() - 3;

			//delta matrix & Mbs (BSPline matrix)
			const unsigned step = 20;
			const auto e = bSplineDelta(1.0/step);
			const auto mbSpline = bSpline();

    		while (i < segments){
    			Coordinate<3> p0 = coordinates_[i];
    			Coordinate<3> p1 = coordinates_[i+1];
    			Coordinate<3> p2 = coordinates_[i+2];
    			Coordinate<3> p3 = coordinates_[i+3];

    			auto gx = Matrix<4,1>{
    				p0.values()[0],
					p1.values()[0],
					p2.values()[0],
					p3.values()[0],
    			};

    			auto gy = Matrix<4,1>{
					p0.values()[1],
					p1.values()[1],
					p2.values()[1],
					p3.values()[1],
				};

    			auto gz = Matrix<4,1>{
					p0.values()[2],
					p1.values()[2],
					p2.values()[2],
					p3.values()[2],
				};


    			//coefficient matrices
    			auto cx = mbSpline * gx;
    			auto cy = mbSpline * gy;
    			auto cz = mbSpline * gz;

    			//forward differences matrices
    			auto fdx = e * cx;
    			auto fdy = e * cy;
    			auto fdz = e * cz;

    			forwardDifference(step,
    					fdx(0,0), fdx(1,0), fdx(2,0), fdx(3,0),
    					fdy(0,0), fdy(1,0), fdy(2,0), fdy(3,0),
    					fdz(0,0), fdz(1,0), fdz(2,0), fdz(3,0));

    			i++;
    		}
    	}


        return points_;
    }

    void BSpline::forwardDifference(unsigned n,
    		               double x, double dx, double d2x, double d3x,
    		               double y, double dy, double d2y, double d3y,
    		               double z, double dz, double d2z, double d3z) {

    	int i = 0;
    	while ( i < n) {
    		x   += dx;
    		dx  += d2x;
    		d2x += d3x;

    		y   += dy;
    		dy  += d2y;
    		d2y += d3y;

    		z   += dz;
    		dz  += d2z;
    		d2z += d3z;

    		points_.push_back(Coordinate<3>{x,y,z});

    		i++;
    	}

    }

    const std::vector<Coordinate<3>> &BSpline::originalCoordinates()
    {
    	return coordinates_;
    }


    GraphicType BSpline:: type(){
    	return BSPLINE;
    }

    bool BSpline::isEmpty() const
    {
    	return coordinates_.size() == 0;
    }

    void BSpline::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    	points_ = {};
    }
}

