#ifndef WORLD_H
#define WORLD_H

#include "GraphicObject.h"
#include "Matrix.h"

#include <map>
#include <memory>
#include <string>

using namespace std;

namespace model {
    class GraphicObject;

    class World {
        public:
            World();
            void addObject(const GraphicObject &go);
            void addObject(std::unique_ptr<GraphicObject> go);
            int countObjects() const;
            std::vector<std::unique_ptr<GraphicObject>> objects() const;
            void transform(const std::string name, utils::Matrix<4,4> transformation);
            Coordinate<3> center(const std::string name);
            void clear();
			GraphicType objectType(const string name);

        private:
            map<string, unique_ptr<GraphicObject>> displayFile;
            int countObjects_;
    };
}

#endif
