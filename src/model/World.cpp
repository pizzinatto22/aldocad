#include "World.h"


#include "GraphicObject.h"


namespace model {
    World::World():
    countObjects_{0}
    {}

    void World::addObject(const GraphicObject &go)
    {
    	std::string name = go.name();
    	displayFile[name] = clone(go);
        countObjects_++;
    }

    void World::addObject(std::unique_ptr<GraphicObject> go)
    {
    	displayFile[go->name()] = move(go);
        countObjects_++;
    }

    int World::countObjects() const
    {
    	return this->countObjects_;
    }

    vector<unique_ptr<GraphicObject>> World::objects() const
    {
    	auto objects = std::vector<unique_ptr<GraphicObject>>{};

    	for (auto &object : displayFile) {
    		objects.push_back(clone(*object.second));
    	}

    	return objects;
    }

    void World::transform(const std::string name, utils::Matrix<4,4> transformation)
    {
    	displayFile.at(name)->transform(transformation);
    }

    Coordinate<3> World::center(const std::string name)
    {
    	return displayFile.at(name)->center();
    }

    void World::clear()
    {
    	displayFile.clear();
    }

	GraphicType World::objectType(const string name){
		return displayFile.at(name)->type();
	}
}
