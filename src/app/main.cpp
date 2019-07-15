#include "Facade.h"
#include "Line.h"
#include "Object3D.h"
#include "MainView.h"
#include "ViewPort.h"
#include "Window.h"
#include "World.h"

#include <QtWidgets/QApplication>

using namespace model;
using namespace controller;

int main(int argc, char *argv[])
{
	World *world = new model::World();

	Object3D trunk = Object3D("trunk",
			{
					Coordinate<3>(10.0,10.0,0.0),Coordinate<3>(100.0,10.0,0.0),
					Coordinate<3>(100.0,10.0,0.0),Coordinate<3>(100.0,100.0,0.0),
					Coordinate<3>(100.0,100.0,0.0),Coordinate<3>(10.0,100.0,0.0),
					Coordinate<3>(10.0,100.0,0.0),Coordinate<3>(10.0,10.0,0.0),

					Coordinate<3>(40.0,70.0,100.0),Coordinate<3>(40.0,40.0,100.0),
					Coordinate<3>(40.0,40.0,100.0),Coordinate<3>(70.0,40.0,100.0),
					Coordinate<3>(70.0,40.0,100.0),Coordinate<3>(70.0,70.0,100.0),
					Coordinate<3>(70.0,70.0,100.0),Coordinate<3>(40.0,70.0,100.0),

					Coordinate<3>(10.0,100.0,0.0),Coordinate<3>(40.0,70.0,100.0),
					Coordinate<3>(10.0,10.0,0.0),Coordinate<3>(40.0,40.0,100.0),
					Coordinate<3>(100.0,10.0,0.0),Coordinate<3>(70.0,40.0,100.0),
					Coordinate<3>(100.0,100.0,0.0),Coordinate<3>(70.0,70.0,100.0)

			});

	Object3D cube = Object3D("cube",
			{
					Coordinate<3>(10.0,10.0,0.0),Coordinate<3>(100.0,10.0,0.0),
					Coordinate<3>(100.0,10.0,0.0),Coordinate<3>(100.0,100.0,0.0),
					Coordinate<3>(100.0,100.0,0.0),Coordinate<3>(10.0,100.0,0.0),
					Coordinate<3>(10.0,100.0,0.0),Coordinate<3>(10.0,10.0,0.0),

					Coordinate<3>(10.0,10.0,100.0),Coordinate<3>(100.0,10.0,100.0),
					Coordinate<3>(100.0,10.0,100.0),Coordinate<3>(100.0,100.0,100.0),
					Coordinate<3>(100.0,100.0,100.0),Coordinate<3>(10.0,100.0,100.0),
					Coordinate<3>(10.0,100.0,100.0),Coordinate<3>(10.0,10.0,100.0),

					Coordinate<3>(10.0,100.0,0.0),Coordinate<3>(10.0,100.0,100.0),
					Coordinate<3>(10.0,10.0,0.0),Coordinate<3>(10.0,10.0,100.0),
					Coordinate<3>(100.0,10.0,0.0),Coordinate<3>(100.0,10.0,100.0),
					Coordinate<3>(100.0,100.0,0.0),Coordinate<3>(100.0,100.0,100.0)

			});


	world->addObject(trunk);
	world->addObject(cube);
	world->addObject(Line{"x", 0.0, 400.0, 0.0, -400.0});
	world->addObject(Line{"y", 400.0, 0.0, -400.0, 0.0});
	Window *window = new Window(world, 410,340);
	ViewPort *viewport = new ViewPort(window, 360,10);

	Facade *facade = new Facade(world,window,viewport);

    QApplication app{argc, argv};
    gui::MainView mainView{facade};

    facade->defineUIViewport(mainView.viewportUI());
    facade->defineUIObjectTable(mainView.objectsTableUI());

    facade->updateViewport();

    mainView.show();
    return app.exec();
}
