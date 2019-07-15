#include "BSpline.h"
#include "GraphicObject.h"
#include "Line.h"
#include "Matrix.h"
#include "Object3D.h"
#include "Point.h"
#include "Polygon.h"
#include "Surface.h"
#include "ViewPort.h"

using namespace utils;

namespace model {
    ViewPort::ViewPort(Window *window, double height, double offset):
		window_{window},
    	height_{height},
    	offset_{offset}
    {}

    vector<unique_ptr<GraphicObject>> ViewPort::transformedObjects() const
    {

    	vector<unique_ptr<GraphicObject>> df = window_->transformedObjects();
    	vector<unique_ptr<GraphicObject>> transformed;

    	auto transformation = Matrix<4,4>{
    		1.0,                     0.0, 0.0, 0.0,
    		0.0,                    -1.0, 0.0, 0.0,
    		0.0,                     0.0, 1.0, 0.0,
    		offset_, (height_ - offset_), 0.0, 1.0
    	};

    	for (auto & item : df){
    		std::vector<Coordinate<3>> oldCoordinates = item->coordinates();
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

    		model::Point p = Point(item->name());
    		model::Line l = Line(item->name());
    		model::Polygon pl = Polygon(item->name(),{});
    		model::BSpline bs = BSpline(item->name(),{});
    		model::Object3D o3d = Object3D(item->name(),{});
    		model::Surface s = Surface(item->name(),0,{},{});

    		switch (item->type()) {
				case POINT:
					if (newCoordinates.size() > 0)
						p = Point(item->name(),
								newCoordinates[0].values()[0],
								newCoordinates[0].values()[1]);

					transformed.push_back(clone(p));

					break;
				case LINE:
					if (newCoordinates.size() > 0)
						l = Line(item->name(),
								newCoordinates[0].values()[0],
								newCoordinates[0].values()[1],
								newCoordinates[1].values()[0],
								newCoordinates[1].values()[1]);
					transformed.push_back(clone(l));

					break;

				case POLYGON:
					if (newCoordinates.size() > 0)
						pl = Polygon(item->name(),newCoordinates);

					transformed.push_back(clone(pl));
					break;
				case BSPLINE:
					if (newCoordinates.size() > 0)
						bs = BSpline(item->name(),newCoordinates);

					transformed.push_back(clone(bs));
					break;
				case OBJECT3D:
					if (newCoordinates.size() > 0)
						o3d = Object3D(item->name(),newCoordinates);

					transformed.push_back(clone(o3d));
					break;
				case SURFACE:
					if (newCoordinates.size() > 0)
						s = Surface(item->name(),0,item->originalCoordinates(),newCoordinates);

					transformed.push_back(clone(s));
					break;
			}

    	}

    	return transformed;
    }

}


