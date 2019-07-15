#include "Matrix.h"
#include "Surface.h"

#include <iostream>


using namespace std;


namespace model {
	using namespace utils;

    Surface::Surface(const std::string &name, const unsigned lines, const std::vector<Coordinate<3>> &coordinates):
        ClonableBase{name},
        lines_{lines},
        coordinates_{coordinates},
        points_{{}}
    {}

    Surface::Surface(const std::string &name, const unsigned lines, const std::vector<Coordinate<3>> &coordinates, const std::vector<Coordinate<3>> &points):
        ClonableBase{name},
        lines_{lines},
        coordinates_{coordinates},
        points_{points}
    {}


    const std::vector<Coordinate<3>> &Surface::coordinates()
    {

    	if (points_.size() == 0 && !isEmpty() && lines_>0){

    		//generate lines in t direction
    		int segments = (coordinates_.size()/lines_) - 3;

			for (int line = 0 ; line < lines_; line++){

				int i = 0;

				while (i < segments){
					Coordinate<3> p0 = coordinates_[(line * lines_) + i];
					Coordinate<3> p1 = coordinates_[(line * lines_) + i+1];
					Coordinate<3> p2 = coordinates_[(line * lines_) + i+2];
					Coordinate<3> p3 = coordinates_[(line * lines_) + i+3];

					generatePoints(p0, p1, p2, p3);

					i++;
				}
			}

			//generate lines in s direction
			segments = (points_.size()/lines_);
			int lines = lines_ - 3;

			for (int i = 0; i < segments; i++){

				int line = 0;

				while (line < lines){
					Coordinate<3> p0 = points_[((line+0) * segments) + i];
					Coordinate<3> p1 = points_[((line+1) * segments) + i];
					Coordinate<3> p2 = points_[((line+2) * segments) + i];
					Coordinate<3> p3 = points_[((line+3) * segments) + i];

					generatePoints(p0, p1, p2, p3);

					line++;
				}
			}
    	}


        return points_;
    }

    void Surface::generatePoints(Coordinate<3> p0, Coordinate<3> p1, Coordinate<3> p2, Coordinate<3> p3)
    {
		//delta matrix & Mbs (BSPline matrix)
        const unsigned step = 10;
		const auto e = utils::bSplineDelta(1.0/step);
		const auto mbSpline = utils::bSpline();


    	auto gx = Matrix<4,1>{
			p0(0),
			p1(0),
			p2(0),
			p3(0),
		};

		auto gy = Matrix<4,1>{
			p0(1),
			p1(1),
			p2(1),
			p3(1),
		};

		auto gz = Matrix<4,1>{
			p0(2),
			p1(2),
			p2(2),
			p3(2),
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
    }

    void Surface::forwardDifference(unsigned n,
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

    const std::vector<Coordinate<3>> &Surface::originalCoordinates()
    {
    	return coordinates_;
    }


    GraphicType Surface:: type(){
    	return SURFACE;
    }

    bool Surface::isEmpty() const
    {
    	return coordinates_.size() == 0;
    }

    const unsigned Surface::lines() const
    {
    	return lines_;
    }

    void Surface::changeCoordinates(std::vector<Coordinate<3>> newCoordinates)
    {
    	coordinates_ = newCoordinates;
    	points_ = {};
    	coordinates(); //just to generate points
    }
}

