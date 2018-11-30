#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H
#include <string>
#include <Sources/Engine/Model.h>
#include <map>
#include <filesystem>
#include <dirent.h>


class AssetsManager
{
public:
    AssetsManager();
    AssetsManager(const std::string& assets_folder);
    Model * getModelByAlias(std::string& alias);
    std::vector<std::string> _get_directories(const std::string& directory);
private:
    void loadAllAssets(const std::string& assets_folder);
    std::map<std::string, Model *> model_map;
    int loaded_models_count;
};



#endif // ASSETSMANAGER_H
