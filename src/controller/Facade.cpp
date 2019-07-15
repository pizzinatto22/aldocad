#define _USE_MATH_DEFINES


#include "csv.h"
#include "BSpline.h"
#include "Facade.h"
#include "GraphicObject.h"
#include "Line.h"
#include "make_unique.h"
#include "Object3D.h"
#include "Point.h"
#include "Polygon.h"
#include "Surface.h"
#include "tiny_obj_loader.h"


#include <iostream>
#include <libgen.h>
#include <map>
#include <math.h>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPolygonF>
#include <QtWidgets/QMessageBox>
#include <string>
#include <tuple>
#include <utility>

#define rad (180/M_PI)

using utils::make_unique;
using namespace std;
using namespace utils;

namespace controller {

	Facade::Facade(World *world, Window *window, ViewPort *viewport):
	world_{world},
	window_{window},
	viewport_{viewport}
	{}

	bool Facade::addPoint(const char *x, const char *y)
	{

    	if (validateDoubleWithMessage(x) &&
    			validateDoubleWithMessage(y)){
    		int qt = world_->countObjects();

    		string name = "object";
    		name.append(to_string(qt));

    		auto point = make_unique<Point>(name,atof(x),atof(y));
    		world_->addObject(move(point));

        	updateViewport();
    		return true;
    	}


    	return false;
	}

	bool Facade::addLine(const char *x1, const char *y1, const char *x2, const char *y2){

		if (validateDoubleWithMessage(x1) &&
				validateDoubleWithMessage(y1) &&
				validateDoubleWithMessage(x2) &&
				validateDoubleWithMessage(y2)){

    		int qt = world_->countObjects();

    		string name = "object";
    		name.append(to_string(qt));

    		auto line = make_unique<Line>(name,atof(x1),atof(y1),atof(x2),atof(y2));
    		world_->addObject(move(line));

        	updateViewport();
    		return true;
		}

		return false;
	}

	bool Facade::validateDouble(const char *value)
	{
		QDoubleValidator validator;

    	int i = 0;

    	QString s =  QString::fromStdString(value);

    	bool ret = validator.validate(s,i) == QValidator::Acceptable;

    	return ret;
	}

	bool Facade::validateInt(const char *value)
	{
		QIntValidator validator;

    	int i = 0;

    	QString s =  QString::fromStdString(value);

    	bool ret = validator.validate(s,i) == QValidator::Acceptable;

    	return ret;
	}



	bool Facade::validateDoubleWithMessage(const char *value)
	{
		if (!validateDouble(value)){
			char buff[100];
			sprintf(buff, "Incorred double value: %s", value);
			message(string(buff));
			return false;
		}

		return true;
	}

	bool Facade::validateIntWithMessage(const char *value)
	{
		if (!validateInt(value)){
			char buff[100];
			sprintf(buff, "Incorred int value: %s", value);
			message(string(buff));
			return false;
		}

		return true;
	}


	void Facade::message(string message)
	{
		QMessageBox msgBox;
		msgBox.setText(message.c_str());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();

	}

	vector<Coordinate<3>> Facade::transformVector(vector<tuple<string, string, string>> coordinates) {
		vector<Coordinate<3>> doubleCoordinates;

		for (int i = 0; i < coordinates.size(); i++) {

			auto t = coordinates[i];

			const char *x = get<0>(t).c_str();
			const char *y = get<1>(t).c_str();
			const char *z = get<2>(t).c_str();

			if (!validateDoubleWithMessage(x)
					|| !validateDoubleWithMessage(y)
					|| !validateDoubleWithMessage(z))
				return {};

			Coordinate<3> c = { atof(x), atof(y), atof(z)};

			doubleCoordinates.push_back(c);
		}

		return doubleCoordinates;
	}

	bool Facade::addPolygon(vector<tuple<string, string, string>> coordinates)
	{

		vector<Coordinate<3>> doubleCoordinates = transformVector(coordinates);

		if (doubleCoordinates.size() == 0)
			return false;

		int qt = world_->countObjects();

		string name = "object";
		name.append(to_string(qt));

		auto polygon = make_unique<Polygon>(name,doubleCoordinates);
		world_->addObject(move(polygon));

    	updateViewport();

		return true;
	}

	bool Facade::addBSpline(vector<tuple<string, string, string>> coordinates)
	{
		if (coordinates.size() < 4){
			message("BSpline requires at least 4 points. Check and try again.");
			return false;
		}

		vector<Coordinate<3>> doubleCoordinates = transformVector(coordinates);

		if (doubleCoordinates.size() == 0)
			return false;

		int qt = world_->countObjects();

		string name = "object";
		name.append(to_string(qt));

		auto bSpline = make_unique<BSpline>(name,doubleCoordinates);
		world_->addObject(move(bSpline));

    	updateViewport();

		return true;

	}

	bool Facade::addObject3D(vector<tuple<string, string, string>> coordinates)
	{

		vector<Coordinate<3>> doubleCoordinates = transformVector(coordinates);

		if (doubleCoordinates.size() == 0)
			return false;

		int qt = world_->countObjects();

		string name = "object";
		name.append(to_string(qt));

		auto obj = make_unique<Object3D>(name,doubleCoordinates);
		world_->addObject(move(obj));

    	updateViewport();

		return true;
	}

	bool Facade::addSurface(vector<tuple<string, string, string, string>> coordinates)
	{

		map <int,vector<tuple<string, string, string>>> pointsPerLine;

		//group points per line
		for (int i = 0; i < coordinates.size(); i++) {

			auto t = coordinates[i];

			const char *line = get<0>(t).c_str();

			if (!validateIntWithMessage(line))
				return false;

			int pos = atoi(line);

			string x = get<1>(t);
			string y = get<2>(t);
			string z = get<3>(t);

			if (pointsPerLine.find(pos) == pointsPerLine.end()){
				pointsPerLine[pos] = {make_tuple(x,y,z)};
			} else{
				vector<tuple<string, string, string>> &list = pointsPerLine[pos];
				list.push_back(make_tuple(x,y,z));
			}
		}

		vector<Coordinate<3>> newCoordinates = {};

		//check if all lines have same point amount
		int size = pointsPerLine.begin()->second.size();
		for (auto c:pointsPerLine){

			int cSize = c.second.size();
			if (cSize != size){
				message("In surface creation, each line must have same point amount. Line " + to_string(c.first) + " is wrong. Actual size: " + to_string(cSize) + "; expected " + to_string(size) + ".");
				return false;
			}

			vector<Coordinate<3>> transformed = transformVector(c.second);

			if (transformed.size() == 0)
				return false;

			newCoordinates.insert(newCoordinates.end(),transformed.begin(),transformed.end());
		}

		for (auto c :newCoordinates){
			cout << " x = " << c(0) << " y=" << c(1) << " z=" << c(2) << endl;
		}

		if (pointsPerLine.size() < 4 || size < 4){
			message("In surface creation, we need at least 4 lines (each one with at least 4 points).");
			return false;
		}



		int qt = world_->countObjects();

		string name = "object";
		name.append(to_string(qt));

		auto obj = make_unique<Surface>(name,(unsigned)size,newCoordinates);
		obj->coordinates(); //just to generate coordinates
		world_->addObject(move(obj));

    	updateViewport();

		return true;
	}

	void Facade::defineUIViewport(QGraphicsView *viewport)
	{
		viewportUI_ = viewport;

		QGraphicsScene *scene = new QGraphicsScene();
		viewportUI_->setScene(scene);
	}

	void Facade::defineUIObjectTable(QTableWidget *objectTable)
	{
		objectTable_ = objectTable;
	}

	void Facade::updateViewport()
	{
		QGraphicsScene *scene = viewportUI_->scene();
		scene->clear();

		//updating little table with object's name&type
		objectTable_->clearContents();

		vector<std::unique_ptr<GraphicObject>> df = viewport_->transformedObjects();

    	objectTable_->setRowCount(df.size());

    	int i = 0;

    	for (auto & item : df){
    		string type;

    		switch (item->type()) {
				case POINT:
	    			type = "Point";

	    			if (!item->isEmpty()){
	    				drawPoint(scene,item->coordinates()[0]);
	    			}
					break;
				case LINE:
	    			type = "Line";

	    			if (!item->isEmpty()){
	    				drawLine(scene,item->coordinates()[0],item->coordinates()[1]);
	    			}
	    			break;
				case POLYGON:
				case BSPLINE:
				case OBJECT3D:
				case SURFACE:
					if (item->type() == POLYGON)
						type = "Polygon";
					else if (item->type() == BSPLINE)
						type = "B-Spline";
					else if (item->type() == OBJECT3D)
						type = "Object 3D";
					else
						type = "Surface";

	    			if (!item->isEmpty()){
	    				int size = item->coordinates().size();

	    				if (size == 1) {
	    					drawPoint(scene,item->coordinates()[0]);
	    				} else {
							int k = 0;

							auto coordinates = item->coordinates();

							while (k < size){
								drawLine(scene,coordinates[k],coordinates[k+1]);
								k = k + 2;
							}
	    				}
	    			}
					break;
			}

    		objectTable_->setItem(i,0,new QTableWidgetItem(item->name().c_str()));
    		objectTable_->setItem(i,1,new QTableWidgetItem(type.c_str()));

    		i++;
    	}

    	viewportUI_->setScene(scene);
	}

	void Facade::changeLineClippingAlgorithm(int option)
	{
		if (option == 0)
			window_->changeLineClippingAlgorithm(COHEN_SUTHERLAND);
		else
			window_->changeLineClippingAlgorithm(LIANG_BARSKY);

		updateViewport();
	}

	void Facade::changeProjectionAlgorithm(int option)
	{
		if (option == 0)
			window_->changeProjectionAlgorithm(PARALLEL);
		else
			window_->changeProjectionAlgorithm(PERSPECTIVE);

		updateViewport();
	}

	void Facade::changeProjectionDistance(int i)
	{
		window_->changeProjectionDistance(i * 1.0);
		updateViewport();
	}

	void Facade::clearWorld()
	{
		world_->clear();

		updateViewport();
	}

	void Facade::zoomIn()
	{
		window_->zoomIn();
		updateViewport();
	}

	void Facade::zoomOut()
	{
		window_->zoomOut();
		updateViewport();
	}

	void Facade::up()
	{
		window_->up();
		updateViewport();
	}
	void Facade::down()
	{
		window_->down();
		updateViewport();
	}

	void Facade::left()
	{
		window_->left();
		updateViewport();
	}

	void Facade::right()
	{
		window_->right();
		updateViewport();
	}

	bool Facade::translateObject(string object,const char *dx, const char *dy, const char *dz)
	{
		if (validateDoubleWithMessage(dx) &&
				validateDoubleWithMessage(dy) &&
				validateDoubleWithMessage(dz)) {

			utils::Matrix <1,4> m;
			m(0,0) = atof(dx);
			m(0,1) = atof(dy);
			m(0,2) = atof(dz);
			m(0,3) = 1;

			utils::Matrix<4,4> transformation = utils::Matrix<4,4>::translation(m);

			world_->transform(object,transformation);

			updateViewport();

			return true;
		}
		return false;
	}

	bool Facade::scaleObject(string object,const char *dx, const char *dy, const char *dz)
		{
			if (validateDoubleWithMessage(dx) &&
					validateDoubleWithMessage(dy) &&
					validateDoubleWithMessage(dz)) {

				utils::Matrix <1,4> d;
				d(0,0) = atof(dx);
				d(0,1) = atof(dy);
				d(0,2) = atof(dz);
				d(0,3) = 1;

				utils::Matrix<4,4> scale = utils::Matrix<4,4>::diagonalize(d);

				Coordinate<3> center = world_->center(object);

				d(0,0) = -center.values()[0];
				d(0,1) = -center.values()[1];
				d(0,2) = -center.values()[2];
				d(0,3) = 1;

				utils::Matrix <4,4> translateCenter = utils::Matrix<4,4>::translation(d);

				d(0,0) = center.values()[0];
				d(0,1) = center.values()[1];
				d(0,2) = center.values()[2];
				d(0,3) = 1;

				utils::Matrix <4,4> translateBack = utils::Matrix<4,4>::translation(d);

				utils::Matrix <4,4> finalScaleMatrix = translateCenter * scale * translateBack;

				world_->transform(object,finalScaleMatrix);

				updateViewport();
				return true;
			}

			return false;
		}

		bool Facade::rotateObjectOrigin(string object,const char *degrees){
			if (validateDoubleWithMessage(degrees)){

				double d = atof(degrees)/rad;
				utils::Matrix<4,4> id = utils::Matrix<4,4>::identity();

				rotateObject(object,d,id,id);

				updateViewport();
				return true;
			}
			return false;
		}

		bool Facade::rotateObjectCenter(string object,const char *degrees){
			if (validateDoubleWithMessage(degrees)){

				double deg = atof(degrees)/rad;
				utils::Matrix <1,4> d;

				Coordinate<3> center = world_->center(object);

				d(0,0) = -center.values()[0];
				d(0,1) = -center.values()[1];
				d(0,2) = 0.0; //z
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateCenter = utils::Matrix<4,4>::translation(d);

				d(0,0) = center.values()[0];
				d(0,1) = center.values()[1];
				d(0,2) = 0.0;
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateBack = utils::Matrix<4,4>::translation(d);

				rotateObject(object,deg,translateCenter,translateBack);


				updateViewport();
				return true;
			}


			return false;
		}

		bool Facade::rotateObjectPoint(string object,const char *degrees,const char *x, const char *y){
			if (validateDoubleWithMessage(degrees) &&
					validateDoubleWithMessage(x) &&
					validateDoubleWithMessage(y)){

				double deg = atof(degrees)/rad;
				double dx = atof(x);
				double dy = atof(y);

				utils::Matrix <1,4> d;


				d(0,0) = -dx;
				d(0,1) = -dy;
				d(0,2) = 0;
				d(0,3) = 1;

				utils::Matrix <4,4> translateCenter = utils::Matrix<4,4>::translation(d);

				d(0,0) = dx;
				d(0,1) = dy;
				d(0,2) = 0;
				d(0,3) = 1;

				utils::Matrix <4,4> translateBack = utils::Matrix<4,4>::translation(d);

				rotateObject(object,deg,translateCenter,translateBack);


				updateViewport();
				return true;
			}

			return false;
		}

		void Facade::centerWindow()
		{
			window_->center();

			updateViewport();
		}


		bool Facade::rotateObject3DOrigin(string object, const char *dx, const char *dy, const char *dz)
		{
			if (validateDoubleWithMessage(dx) && validateDoubleWithMessage(dy) && validateDoubleWithMessage(dz)){
				double x = atof(dx)/rad;
				double y = atof(dy)/rad;
				double z = atof(dz)/rad;

				utils::Matrix<4,4> id = utils::Matrix<4,4>::identity();

				rotateObject(object,x,y,z,id,id);

				updateViewport();
				return true;
			}

			return false;
		}

		bool Facade::rotateObject3DCenter(string object, const char *dx, const char *dy, const char *dz)
		{
			if (validateDoubleWithMessage(dx) && validateDoubleWithMessage(dy) && validateDoubleWithMessage(dz)){
				double x = atof(dx)/rad;
				double y = atof(dy)/rad;
				double z = atof(dz)/rad;

				utils::Matrix <1,4> d;

				Coordinate<3> center = world_->center(object);


				d(0,0) = -center.values()[0];
				d(0,1) = -center.values()[1];
				d(0,2) = -center.values()[2];
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateCenter = utils::Matrix<4,4>::translation(d);

				d(0,0) = center.values()[0];
				d(0,1) = center.values()[1];
				d(0,2) = center.values()[2];
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateBack = utils::Matrix<4,4>::translation(d);

				rotateObject(object,x,y,z,translateCenter,translateBack);

				updateViewport();
				return true;
			}

			return false;
		}

		bool Facade::rotateObject3DPoint(string object, const char *dx, const char *dy, const char *dz, const char *x, const char *y, const char *z)
		{
			if (validateDoubleWithMessage(dx) && validateDoubleWithMessage(dy) && validateDoubleWithMessage(dz)
					&& validateDoubleWithMessage(x) && validateDoubleWithMessage(y) && validateDoubleWithMessage(z)){

				double ddx = atof(dx)/rad;
				double ddy = atof(dy)/rad;
				double ddz = atof(dz)/rad;

				double xx = atof(x);
				double yy = atof(y);
				double zz = atof(z);


				utils::Matrix <1,4> d;

				d(0,0) = -xx;
				d(0,1) = -yy;
				d(0,2) = -zz;
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateCenter = utils::Matrix<4,4>::translation(d);

				d(0,0) = xx;
				d(0,1) = yy;
				d(0,2) = zz;
				d(0,3) = 1.0;

				utils::Matrix <4,4> translateBack = utils::Matrix<4,4>::translation(d);

				rotateObject(object,ddx,ddy,ddz,translateCenter,translateBack);

				updateViewport();
				return true;
			}
			return false;
		}


		void Facade::rotateObject(string object, double degrees, utils::Matrix<4,4> translate1,utils::Matrix<4,4> translate2){

			//2D rotation is equals to 3D rotation over z-axis
			auto t = utils::Matrix <4,4>{
				cos(degrees), sin(degrees), 0.0, 0.0,
			   -sin(degrees), cos(degrees), 0.0, 0.0,
			             0.0,          0.0, 1.0, 0.0,
			             0.0,          0.0, 0.0, 1.0
			};

			world_->transform(object,translate1*t*translate2);
		}

		void Facade::rotateObject(string object, double dx, double dy, double dz, utils::Matrix<4,4> translate1,utils::Matrix<4,4> translate2)
		{
			auto rx = utils::Matrix<4,4>{
				1.0,     0.0,     0.0, 0.0,
				0.0, cos(dx), sin(dx), 0.0,
				0.0,-sin(dx), cos(dx), 0.0,
				0.0,     0.0,     0.0, 1.0
			};

			auto ry = utils::Matrix<4,4>{
				cos(dy), 0.0,-sin(dy), 0.0,
				    0.0, 1.0,     0.0, 0.0,
				sin(dy), 0.0, cos(dy), 0.0,
				    0.0, 0.0,     0.0, 1.0
			};

			auto rz = utils::Matrix<4,4>{
				cos(dz), sin(dz), 0.0, 0.0,
			   -sin(dz), cos(dz), 0.0, 0.0,
			        0.0,     0.0, 1.0, 0.0,
			        0.0,     0.0, 0.0, 1.0
			};

			world_->transform(object,translate1*rx*ry*rz*translate2);

		}

		bool Facade::rotateWorld(const char *degrees)
		{
			if (validateDoubleWithMessage(degrees)){

				window_->rotate(atof(degrees));
				updateViewport();
				return true;
			}
			return false;
		}

		void Facade::drawPoint(QGraphicsScene *scene, Coordinate<3> p)
		{
			double r = 1;
			double x0 = p.values()[0];
			double y0 = p.values()[1];

			scene->addEllipse(x0 - r, y0 - r, r*2,r*2);
		}

		void Facade::drawLine(QGraphicsScene *scene, Coordinate<3> p1,Coordinate<3> p2)
		{
			double x0 = p1.values()[0];
			double y0 = p1.values()[1];
			double x1 = p2.values()[0];
			double y1 = p2.values()[1];

			scene->addLine(x0,y0,x1,y1);

		}

		GraphicType Facade::objectType(string name)
		{
			return world_->objectType(name);
		}

		void Facade::loadFile(string fileName) {
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;

			std::string err = tinyobj::LoadObj(shapes, materials, fileName.c_str());

			if (!err.empty()) {
				message(err);
				return;
			}

			vector<Coordinate<3>> doubleCoordinates;

			for (size_t i = 0; i < shapes.size(); i++) {
				for (size_t f = 0; f < shapes[i].mesh.indices.size(); f = f + 3) {
					int p0 = shapes[i].mesh.indices[f];
					int p1 = shapes[i].mesh.indices[f + 1];
					int p2 = shapes[i].mesh.indices[f + 2];

					Coordinate<3> c1 = Coordinate<3> { shapes[i].mesh.positions[3 * p0
							+ 0], shapes[i].mesh.positions[3 * p0 + 1],
							shapes[i].mesh.positions[3 * p0 + 2] };
					Coordinate<3> c2 = Coordinate<3> { shapes[i].mesh.positions[3 * p1
							+ 0], shapes[i].mesh.positions[3 * p1 + 1],
							shapes[i].mesh.positions[3 * p1 + 2] };
					Coordinate<3> c3 = Coordinate<3> { shapes[i].mesh.positions[3 * p2
							+ 0], shapes[i].mesh.positions[3 * p2 + 1],
							shapes[i].mesh.positions[3 * p2 + 2] };

					doubleCoordinates.push_back(c1);
					doubleCoordinates.push_back(c2);
					doubleCoordinates.push_back(c2);
					doubleCoordinates.push_back(c3);
					doubleCoordinates.push_back(c3);
					doubleCoordinates.push_back(c1);
				}
			}

			Object3D obj = Object3D(string(basename((char*) fileName.c_str())),
					doubleCoordinates);

			world_->addObject(move(obj));

			updateViewport();
		}

		const vector<vector<string>> Facade::loadCSV(string fileName)
		{
			return csv(fileName);
		}


}
