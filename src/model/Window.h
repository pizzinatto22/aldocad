#ifndef WINDOW_H
#define WINDOW_H

#include "BSpline.h"
#include "Coordinate.h"
#include "GraphicObject.h"
#include "Line.h"
#include "Matrix.h"
#include "Object3D.h"
#include "Point.h"
#include "Polygon.h"
#include "Surface.h"
#include "World.h"


#include <vector>

namespace model {

	enum LineClippingAlgorithm{
		COHEN_SUTHERLAND,
		LIANG_BARSKY,
	};

	enum ProjectionAlgorithm{
		PARALLEL,
		PERSPECTIVE,
	};

	class GraphicObject;

    class Window {
    public:
        Window(World *world, double width, double height);
        std::vector<std::unique_ptr<GraphicObject>> transformedObjects();
		void zoomIn();
		void zoomOut();
		void up();
		void down();
		void left();
		void right();
		void rotate(double degrees);
		void center();

		void changeLineClippingAlgorithm(LineClippingAlgorithm newAlgorithm);
		void changeProjectionAlgorithm(ProjectionAlgorithm newAlgorithm);
		void changeProjectionDistance(double d);

    private:
        World *world_;
        double width_;
        double height_;
        double degrees_;
        double scale_;
        utils::Matrix<4, 4> transformation;
        Coordinate<2> center_;
        LineClippingAlgorithm algorithm;
        ProjectionAlgorithm projection;
        double projectionDistance;

		void translate(double dx, double dy);
		void scale(double factor);
		Coordinate<2> scaledCenter();

		//***clipping
		model::Point pointClip(model::Point p) const;
		model::Line lineClip(model::Line l) const;
		model::Polygon polygonClip(model::Polygon p) const;
		model::BSpline bSplineClip(model::BSpline p) const;
		model::Object3D object3dClip(model::Object3D o) const;
		model::Surface surfaceClip(model::Surface s) const;

		model::Line cohenSutherlandLineClip(model::Line l) const;
		int cohenSutherlandLineComputeOutCode(Coordinate<3> p) const;
		model::Line liangBarskLineClip(model::Line l) const;
		int liangBarskLineClipT(double p, double q, double *u1, double *u2) const;
		//***

		model::Object3D project(model::Object3D o);

		double xmin() const;
		double xmax() const;
		double ymin() const;
		double ymax() const;
    };
}

#endif

