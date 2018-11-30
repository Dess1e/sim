#include "AssetsManager.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::AssetsManager(const std::string& assets_folder)
{
    this->loadAllAssets(assets_folder);
}

void AssetsManager::loadAllAssets(const std::string& assets_folder)
{
    auto dirs = this->_get_directories(assets_folder);
    int counter = 0;
    for (auto& d : dirs)
    {
        this->model_map[d] = new Model(assets_folder, d);
        counter++;
    }
    this->loaded_models_count = counter;
}

#ifdef __linux__
std::vector<std::string> AssetsManager::_get_directories(const std::string& directory)
{
    std::vector<std::string> result;
    DIR * d = opendir(directory.c_str());
    dirent * dir;
    if (d)
        while ((dir = readdir(d)) != nullptr)
        {
            std::string dir_name(dir->d_name);
            if (dir_name == ".." || dir_name == ".")
                continue;
            result.push_back(dir_name);
        }
    return result;
}
#endif

#ifdef WIN32
std::vector<std::string> AssetsManager::_get_directories(const std::string& directory)
{
    assert(false);
}
#endif

Model * AssetsManager::getModelByAlias(std::string& alias)
{
    return this->model_map[alias];
}
