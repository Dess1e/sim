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
    std::ifstream infile(assets_folder + "res_list.txt");
    std::string filename, alias;
    while(infile >> filename >> alias)
    {
        const std::string fn_c = filename;
        Model * new_model = new Model(fn_c);
        this->model_map[alias] = new_model;

    }
    infile.close();
}

Model * AssetsManager::getModelByAlias(std::string& alias)
{
    return this->model_map[alias];
}
