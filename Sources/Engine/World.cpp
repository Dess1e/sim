#include "World.h"
#include <Sources/Engine/Engine.h>

World::World()
{
    //load models
    //load model pos to map
}

ObjectID World::spawnObject(std::string asset_alias, glm::vec3 coords, glm::vec3 rotation, glm::vec3 scale)
{
    std::vector<ObjectID> ids;
    for(std::map<ObjectID, Object *>::iterator it = objects_map.begin(); it != objects_map.end(); ++it)
        ids.push_back(it->first);
    std::sort(ids.begin(), ids.end());
    ObjectID new_id;
    if (ids.empty())
        new_id = 0;
    else
        new_id = ids.back();

    Object * obj = new Object(coords, rotation, scale, asset_alias, new_id);
    this->objects_map[new_id] = obj;
    return new_id;
}

void World::renderAll()
{
    if (this->objects_map.empty())
        return;
    else
        for(std::map<ObjectID, Object *>::iterator it = objects_map.begin(); it != objects_map.end(); ++it)
        {
            it->second->render();
        }

}



