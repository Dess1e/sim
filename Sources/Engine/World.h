#ifndef WORLD_HPP
#define WORLD_HPP
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <string>

class Object;

class World
{
public:
    World();
    void loadWorldFromFile(std::string filename);
    void spawnObject(Object * obj, std::string identifier) { objects_map[identifier] = obj; }
private:
    std::map<std::string, Object *> objects_map;
};

#endif // WORLD_HPP
