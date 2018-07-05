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
#include <GL/glew.h>

class ResourceLoader
{
public:
    ResourceLoader();
    GLuint LoadTextureBMP(const char * image_path);
};

#endif // RESOURCE_LOADER_HPP
