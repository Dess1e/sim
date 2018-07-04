#ifndef RESOURCE_LOADER_HPP
#define RESOURCE_LOADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <src/engine/model.hpp>
#include <GL/glew.h>

class ResourceLoader
{
public:
    ResourceLoader();
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    std::vector<Model> LoadModels(std::string file_list);
    GLuint LoadTextureBMP(const char * image_path);
};

#endif // RESOURCE_LOADER_HPP
