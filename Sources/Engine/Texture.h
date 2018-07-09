#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>


class Texture
{
public:
    Texture();
    unsigned int id;
    std::string type;
    std::string path;
};

#endif // TEXTURE_H
