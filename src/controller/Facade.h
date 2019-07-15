#ifndef FACADE_H_
#define FACADE_H_

#include "Coordinate.h"

#include "ViewPort.h"
#include "Window.h"
#include "World.h"


#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QTableWidget>
#include <vector>
#include <tuple>

using namespace model;
using namespace std;


namespace controller {

	class Facade {
		public:
			Facade(World *world, Window *window, ViewPort *viewport);
			bool addPoint(const char *x, const char *y);
			bool addLine(const char *x1, const char *y1, const char *x2, const char *y2);
			bool addPolygon(vector<tuple<string,string, string>> coordinates);
			bool addBSpline(vector<tuple<string,string, string>> coordinates);
			bool addObject3D(vector<tuple<string,string, string>> coordinates);
			bool addSurface(vector<tuple<string, string,string, string>> coordinates);

			void defineUIViewport(QGraphicsView *viewport);
			void defineUIObjectTable(QTableWidget *objectTable);
			void zoomIn();
			void zoomOut();
			void up();
			void down();
			void left();
			void right();
			bool translateObject(string object,const char *dx, const char *dy, const char *dz);
			bool scaleObject(string object,const char *dx, const char *dy, const char *dz);
			bool rotateObjectOrigin(string object,const char *degrees);
			bool rotateObjectCenter(string object,const char *degrees);
			bool rotateObjectPoint(string object,const char *degrees,const char *x, const char *y);
			void centerWindow();

			bool rotateObject3DOrigin(string object, const char *dx, const char *dy, const char *dz);
			bool rotateObject3DCenter(string object, const char *dx, const char *dy, const char *dz);
			bool rotateObject3DPoint(string object, const char *dx, const char *dy, const char *dz, const char *x, const char *y, const char *z);


			bool rotateWorld(const char *degrees);
			void message(string message);
			void updateViewport();

			void changeLineClippingAlgorithm(int option);
			void changeProjectionAlgorithm(int option);
			void changeProjectionDistance(int i);
			void clearWorld();
			GraphicType objectType(string name);

			void loadFile(string fileName);

			const vector<vector<string>> loadCSV(string fileName);



		private:
			bool validateDouble(const char *value);
			bool validateInt(const char *value);
			bool validateDoubleWithMessage(const char *value);
			bool validateIntWithMessage(const char *value);
			World *world_;
			Window *window_;
			ViewPort *viewport_;
			QGraphicsView *viewportUI_;
			QTableWidget *objectTable_;

			void rotateObject(string object, double degrees, utils::Matrix<4,4> translate1,utils::Matrix<4,4> translate2);
			void rotateObject(string object, double dx, double dy, double dz, utils::Matrix<4,4> translate1,utils::Matrix<4,4> translate2);

			void drawPoint(QGraphicsScene *scene, Coordinate<3> p);
			void drawLine(QGraphicsScene *scene, Coordinate<3> p1,Coordinate<3> p2);

			vector<Coordinate<3>> transformVector(vector<tuple<string, string, string>> coordinates);


	};

}

#endif
