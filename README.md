# AldoCAD
A simple tool for academic purposes to test computer graphics algorithms. This is a basic CAD system (wire-frame objects).
We can create points, lines, surfaces and any kind of 3d objects. Functions available are:
* Object translate, rotate and scale
* Window rotate, movement and zooming (window is a partial view of the world objects)
* _Cohen-Shutterland_ and _Lian-Barsky_ line clipping methods
* Parallel & perspective projections, with custom projection distance (only for 3d objects)
* Everything based on matrix multiplication
* Import obj files

# Authors
Luiz Eduardo Pizzinatto

&

Bruno Martins Crocomo

# Execution
First, install Qt5, then set QTDIR environment variable:
```
brew install qt5
export QTDIR=/usr/local/Cellar/qt/5.13.0/
```

Go to build folder and execute `cmake`:
```
cd build
cmake ../src
```

After, you can build this amazing tool:
```
make
```

If everything goes according to plan, the executable file will be generated inside the `app` folder. Hence, to execute AldoCAD inside `build` folder:
```
.app/app
```

# Examples
This is the initial AldoCAD screen. A few objects are loaded by default.
![system basics](/img/basic.png)

After translate the cube and rotate the trunk, this is the result:
![system functions](/img/rotated_translated.png)

Loading the `extra_files/teapot.obj`, and scaling object 50 times (selecting the object then using `Scale` button):
![teapot](/img/teapot.png)

And rotating this amazing teapot:
![teapot rotated](/img/teapot_rotated.png)

# Infos
