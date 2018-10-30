#ifndef WORLD_HPP
#define WORLD_HPP
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

typedef uint64_t ObjectID;

class Object;

class World
{
public:
    World();
    void loadWorldFromFile(std::string filename);
    void renderAll();
    ObjectID spawnObject(std::string asset_alias, glm::vec3 coords, glm::vec3 rotatiom, glm::vec3 scale);
private:
    std::map<ObjectID, Object *> objects_map;
};

#endif // WORLD_HPP
