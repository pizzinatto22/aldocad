#define _USE_MATH_DEFINES

#include "Matrix.h"
#include "Window.h"

#include <iostream>
#include <math.h>
#include <memory>

#define rad (180/M_PI)

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

using namespace utils;

namespace model {
    Window::Window(World *world, double width, double height):
    	world_{world},
        width_{width},
        height_{height},
        degrees_{0},
        scale_{1.0},
        transformation{decltype(transformation)::identity()},
        center_{Coordinate<2>{width/2.0,height/2.0}},
        algorithm{COHEN_SUTHERLAND},
        projection{PARALLEL},
        projectionDistance{135}
    {}

    vector<unique_ptr<GraphicObject>> Window::transformedObjects()
    {
    	vector<unique_ptr<GraphicObject>> df = world_->objects();

    	vector<unique_ptr<GraphicObject>> transformed;

    	for (auto & item : df){
    		std::vector<Coordinate<3>> oldCoordinates;
    		if (item->type() == OBJECT3D || item->type() == SURFACE){
    			Object3D o{"",item->coordinates()};
    			o = project(o);
    			oldCoordinates = o.coordinates();
    		} else {
    			oldCoordinates = item->coordinates();
    		}
    		std::vector<Coordinate<3>> newCoordinates = {};

    		for (auto c : oldCoordinates){
    			utils::Matrix<1,4> m;
    			m(0,0) = c(0);
    			m(0,1) = c(1);
    			m(0,2) = c(2);
    			m(0,3) = 1.0;

    			m = m * transformation;

    			c = Coordinate<3>(m(0,0),m(0,1),m(0,2));

    			newCoordinates.push_back(c);
    		}

    		model::Point p = Point("");
    		model::Line l = Line("");
    		model::Polygon pl = Polygon("",{});

    		model::BSpline bs = BSpline("",{});
    		model::Object3D o3d = Object3D("",{});
    		model::Surface s = Surface("",0,{},{});

    		switch (item->type()) {
				case POINT:
					p = Point(item->name(),
							newCoordinates[0].values()[0],
							newCoordinates[0].values()[1]);

					p = pointClip(p);

					transformed.push_back(clone(p));

					break;
				case LINE:

					l = model::Line(item->name(),
							newCoordinates[0].values()[0],
							newCoordinates[0].values()[1],
							newCoordinates[1].values()[0],
							newCoordinates[1].values()[1]);

					l = lineClip(l);

					transformed.push_back(clone(l));

					break;

				case POLYGON:
					pl = Polygon(item->name(),newCoordinates);
					pl = polygonClip(pl);
					transformed.push_back(clone(pl));
					break;

				case BSPLINE:
					bs = BSpline(item->name(),item->originalCoordinates(),newCoordinates);
					bs = bSplineClip(bs);
					transformed.push_back(clone(bs));
					break;

				case OBJECT3D:
					o3d = Object3D(item->name(),newCoordinates);
//					o3d = project(o3d);
					o3d = object3dClip(o3d);
					transformed.push_back(clone(o3d));
					break;
				case SURFACE:
					s = Surface(item->name(),0,item->originalCoordinates(),newCoordinates);
					s = surfaceClip(s);
					transformed.push_back(clone(s));
					break;
			}

    	}
    	return transformed;
    }

	void Window::zoomIn()
	{
		scale(1.2);
	}

	void Window::zoomOut()
	{
		scale(1.0/1.2);
	}

	void Window::up()
	{
		translate(0.0,-5.0);
	}

	void Window::down()
	{
		translate(0.0,5.0);
	}

	void Window::left()
	{
		translate(5.0,0.0);
	}

	void Window::right()
	{
		translate(-5.0,0.0);
	}

	void Window::center()
	{
		//rotate center, just to know how much dx&dy we must use to center window in (0,0) world coordinate
		double d = -degrees_/rad;
		double c = cos(d);
		double s = sin(d);

		auto rotateCenter = Matrix<3,3>{
			c,   s, 0.0,
		   -s,   c, 0.0,
		  0.0, 0.0, 1.0
		};

	    auto scaled = scaledCenter();

		Matrix<1,3> actualCenter{
			scaled(0),
			scaled(1),
			1.0
		};
        //now rotate center over origin & discover center vector movement
		Matrix<1,3> centerRotate = actualCenter * rotateCenter; //this is how much actual center must be translated

		translate(centerRotate(0,0),centerRotate(0,1));
	}

	void Window::changeLineClippingAlgorithm(LineClippingAlgorithm newAlgorithm)
	{
		algorithm = newAlgorithm;
	}

	void Window::changeProjectionAlgorithm(ProjectionAlgorithm newAlgorithm)
	{
		projection = newAlgorithm;
	}

	void Window::changeProjectionDistance(double d)
	{
		projectionDistance = d;
	}

	void Window::translate(double dx, double dy)
	{
		auto m = utils::Matrix<1,4>{
			dx,
			dy,
			0.0,
			1.0
		};

		auto translationMatrix = Matrix<4,4>::translation(m);

		transformation = transformation * translationMatrix;


		//move center to the opposite direction, BUT SCALED
		auto cm = utils::Matrix<1,3>{
			-dx/scale_,
			-dy/scale_,
			1.0
		};

		double d = degrees_/rad;
		double c = cos(d);
		double s = sin(d);

		auto rotateCenter = Matrix<3,3>{
			c,   s, 0.0,
		   -s,   c, 0.0,
		  0.0, 0.0, 1.0
		};

        //now rotate center over origin & discover center vector movement
		Matrix<1,3> centerMovement = cm * rotateCenter; //this is how much actual center must be translated

		Matrix<1,3> actualCenter{
			center_(0),
			center_(1),
			1.0
		};

		auto newCenter = actualCenter * Matrix<3,3>::translation(centerMovement);

		center_ = Coordinate<2>(newCenter(0,0), newCenter(0,1));
	}

	void Window::scale(double factor)
	{
		scale_ *= factor;

		utils::Matrix<1,4> m;
		m(0,0) = factor;
		m(0,1) = factor;
		m(0,2) = factor;
		m(0,3) = 1.0;

		auto scale = utils::Matrix<4,4>::diagonalize(m);

		double w = center_(0);
		double h = center_(1);

		//translation
		Matrix <1,4> center1{
			-w,
			-h,
			0.0,
			1.0
		};

		Matrix <1,4> center2{
			w,
			h,
			0.0,
			1.0
		};

		transformation = Matrix<4,4>::translation(center1) * scale * Matrix<4,4>::translation(center2) * transformation;
	}

	Coordinate<2> Window::scaledCenter()
	{
		return Coordinate<2>(center_(0)*scale_,center_(1)*scale_);
	}

	void Window::rotate(double degrees)
	{
		//rotation
		degrees_ += degrees;
		degrees = -degrees / rad;

		auto s = sin(degrees);
		auto c = cos(degrees);

		auto t = Matrix <4,4>{
			   c,   s, 0.0, 0.0,
			  -s,   c, 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			 0.0, 0.0, 0.0, 1.0
		};

		double w = -center_(0);
		double h = -center_(1);

		//translation
		Matrix <1,4> center1{
			-w,
			-h,
			0.0,
			1.0
		};

		Matrix <1,4> center2{
			w,
			h,
			0.0,
			1.0
		};

		t = Matrix<4,4>::translation(center2) * t * Matrix<4,4>::translation(center1);

		transformation = t*transformation;
	}

	model::Point Window::pointClip(model::Point p) const
	{
		auto c = p.coordinates()[0];
		double x = c.values()[0];
		double y = c.values()[1];

		if(xmin() <= x &&
				x <= xmax() &&
				ymin() <= y && y
				<= ymax())
			return p;

		return Point(p.name());
	}

	model::Line Window::lineClip(model::Line l) const{
		if (algorithm == COHEN_SUTHERLAND){
			return cohenSutherlandLineClip(l);
		}
		else {
			return liangBarskLineClip(l);
		}
	}

	model::Line Window::cohenSutherlandLineClip(model::Line l) const
	{
		auto c = l.coordinates();

		int outcode0 = cohenSutherlandLineComputeOutCode(c[0]);
		int outcode1 = cohenSutherlandLineComputeOutCode(c[1]);
		bool accept = false;



		double x0 = c[0].values()[0];
		double y0 = c[0].values()[1];

		double x1 = c[1].values()[0];
		double y1 = c[1].values()[1];

		while (true) {
			if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
				accept = true;
				break;
			} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
				break;
					} else {
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				double x, y;

				// At least one endpoint is outside the clip rectangle; pick it.
				int outcodeOut = outcode0 ? outcode0 : outcode1;

				// Now find the intersection point;
				// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
				if (outcodeOut & TOP) {           // point is above the clip rectangle
					x = x0 + (x1 - x0) * (ymax() - y0) / (y1 - y0);
					y = ymax();
				} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
					x = x0 + (x1 - x0) * (ymin() - y0) / (y1 - y0);
					y = ymin();
				} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
					y = y0 + (y1 - y0) * (xmax() - x0) / (x1 - x0);
					x = xmax();
				} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
					y = y0 + (y1 - y0) * (xmin() - x0) / (x1 - x0);
					x = xmin();
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == outcode0) {
					x0 = x;
					y0 = y;
					outcode0 = cohenSutherlandLineComputeOutCode(Coordinate<3>(x0, y0, 0.0));
				} else {
					x1 = x;
					y1 = y;
					outcode1 = cohenSutherlandLineComputeOutCode(Coordinate<3>(x1, y1, 0.0));
				}
			}
		}


		if (accept) {
			return Line(l.name(),x0,y0,x1,y1);
		}

		return Line(l.name());
	}

	int Window::cohenSutherlandLineComputeOutCode(Coordinate<3> p) const
	{
		int code;

		double x = p.values()[0];
		double y = p.values()[1];

		code = INSIDE;          // initialised as being inside of clip window

		if (x < xmin())           // to the left of clip window
			code |= LEFT;
		else if (x > xmax())      // to the right of clip window
			code |= RIGHT;
		if (y < ymin())           // below the clip window
			code |= BOTTOM;
		else if (y > ymax())      // above the clip window
			code |= TOP;

		return code;
	}

	model::Line Window::liangBarskLineClip(model::Line l) const{
		auto c = l.coordinates();
		double x0 = c[0].values()[0];
		double y0 = c[0].values()[1];

		double x1 = c[1].values()[0];
		double y1 = c[1].values()[1];


		double u1 = 0.0;
		double u2 = 1.0;
		double dx = x1 - x0;
		double dy;

		if(liangBarskLineClipT(-dx, x0 - xmin(), &u1, &u2)){
			if(liangBarskLineClipT(dx, xmax() - x0, &u1, &u2)){
				dy = y1 - y0;
				if(liangBarskLineClipT(-dy, y0 - ymin(), &u1, &u2))
					if(liangBarskLineClipT(dy, ymax() - y0, &u1, &u2)){
						if(u2 < 1.0){
							x1 = x0 + u2 * dx;
							y1 = y0 + u2 * dy;
						}
						if(u1 > 0.0){
							x0 += u1 * dx;
							y0 += u1 * dy;
						}

						return Line(l.name(),x0,y0,x1,y1);
					}
			}
		}

		return Line(l.name());
	}

	int Window::liangBarskLineClipT(double p, double q, double *u1, double *u2) const
	{
		double r;
		int retVal = 1;

		if(p < 0.0){
			r = q / p;
			if(r > *u2)
				retVal = 0;
			else
				if (r > *u1)
					*u1 = r;
		}
		else
			if (p > 0.0){
				r = q / p;
				if (r < *u1)
					retVal = 0;
				else if (r < *u2)
					*u2 = r;
			}
			else
				if(q < 0.0)
					retVal = 0;

			return (retVal);
	}

	model::Polygon Window::polygonClip(model::Polygon p) const
	{

		auto c = p.coordinates();
		int size = c.size();
		if (size > 0) {

			Coordinate<3> c1 = c[0];

			if (size == 1){
				Point p = Point("",c1.values()[0],c1.values()[1]);
				p = pointClip(p);
				if (!p.isEmpty())
					return Polygon(p.name(),{c1});
			} else{
				int i = 1;

				std::vector<Coordinate<3>> newCoordinates = {};

				while (i <= size){
					Coordinate<3> c2 = c[i == size?0:i];

					Line l1 = Line("",c1.values()[0],c1.values()[1],c2.values()[0],c2.values()[1]);

					l1 = lineClip(l1);

					if (!l1.isEmpty()){
						vector<Coordinate<3>> coordinatesLine1 = l1.coordinates();
						newCoordinates.push_back(coordinatesLine1[0]);
						newCoordinates.push_back(coordinatesLine1[1]);
					}

					c1 = c2;
					i++;
				}
				return Polygon(p.name(),newCoordinates);
			}

		}

		return Polygon(p.name(),{});

	}

	model::BSpline Window::bSplineClip(model::BSpline p) const
	{

		//clip as a polygon, but dont use last coordinate
		Polygon aux = Polygon("",p.coordinates());
		aux = polygonClip(aux);

		if (!aux.isEmpty()){

			vector<Coordinate<3>> newCoordinates = aux.coordinates();

			if (newCoordinates.size() > 1)
				newCoordinates.pop_back();

			return BSpline(p.name(),p.originalCoordinates(),newCoordinates);

		}

		return BSpline(p.name(),{});

	}

	model::Object3D Window::object3dClip(model::Object3D o) const
	{

		vector<Coordinate<3>> c = o.coordinates();
		int size = c.size();

		if (size > 0) {

			Coordinate<3> c1 = c[0];

			if (size == 1){
				Point p = Point("",c1.values()[0],c1.values()[1]);
				p = pointClip(p);
				if (!p.isEmpty())
					return Object3D(p.name(),{c1});
			} else{
				int i = 0;

				std::vector<Coordinate<3>> newCoordinates = {};

				while (i < size -1){
					Coordinate<3> c1 = c[i];
					Coordinate<3> c2 = c[i+1];

					Line l1 = Line("",c1.values()[0],c1.values()[1],c2.values()[0],c2.values()[1]);

					l1 = lineClip(l1);

					if (!l1.isEmpty()){

						vector<Coordinate<3>> coordinatesLine1 = l1.coordinates();

						newCoordinates.push_back(coordinatesLine1[0]);

						newCoordinates.push_back(coordinatesLine1[1]);
					}

					i += 2;
				}

				return Object3D(o.name(),newCoordinates);
			}

		}

		return Object3D(o.name(),{});
	}

	Surface Window::surfaceClip(Surface s) const
	{
		Object3D o3d = Object3D("",s.coordinates());
		o3d = object3dClip(o3d);

		return Surface(s.name(),s.lines(),s.originalCoordinates(),o3d.coordinates());
	}

	Object3D Window::project(Object3D o)
	{
		if (projection == PERSPECTIVE){
			vector<Coordinate<3>> newCoordinates;
			vector<Coordinate<3>> coordinates = o.coordinates();

			for (auto c:coordinates){

				double w = center_(0);
				double h = center_(1);

				//translation
				Matrix <1,4> center1{
					-w,
					-h,
					+projectionDistance,
					1.0
				};

				Matrix <1,4> center2{
					w,
					h,
					-projectionDistance,
					1.0
				};

				double inv = 1/projectionDistance;

				Matrix<4,4> mper{
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, inv, 0.0
				};


				Matrix<1,4> realCoordinate{c(0), c(1), c(2), 1.0};

				Matrix<1,4> centered = realCoordinate * Matrix<4,4>::translation(center1);

				if (centered(0,2) < 0){
					centered = Matrix<1,4>{centered(0,0),centered(0,1),0.0,centered(0,3)};
				}

				Matrix<4,1> homogeneous = mper * transpose(centered);

				Matrix<1,4> final = transpose(homogeneous) * Matrix<4,4>::translation(center2);


				Coordinate<3> projected{final(0,0)/final(0,3),
										final(0,1)/final(0,3),
										final(0,2)/final(0,3)};


				newCoordinates.push_back(projected);
			}

			o.changeCoordinates(newCoordinates);
		}

		return o;
	}


	double Window::xmin() const
	{
		return 0;
	}

	double Window::xmax() const
	{
		return width_;
	}


	double Window::ymin() const
	{
		return 0;
	}

	double Window::ymax() const
	{
		return height_;
	}


}


