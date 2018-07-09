#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{

}


GLuint ResourceLoader::LoadTextureBMP(const char * image_path)
{
    unsigned char header[54];
    unsigned int data_pos;
    unsigned int image_size;
    unsigned int width, height;

    unsigned char * data;

    FILE * file = fopen(image_path, "rb");
    if (!file)
    {
        std::cerr << "Can't open file" << std::endl;
        return 0;
    }
    if (fread(header, 1, 54, file) != 54)
    {
        std::cerr << "Header is not 54 bytes" << std::endl;
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Header does not contain B and M bytes" << std::endl;
        return 0;
    }
    if ( *(int*)&(header[0x1E]) != 0)
    {
        std::cerr << "0x1E is not 0" << std::endl;
        return 0;
    }
    if ( *(int*)&(header[0x1C]) != 24)
    {
        std::cerr << "0x1C is not 0" << std::endl;
        return 0;
    }
    data_pos = *(int*)&(header[0x0A]);
    image_size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (!image_size)
        image_size = width * height * 3;
    if (!data_pos)
        data_pos = 54;
    data = new unsigned char[image_size];
    fread(data, 1, image_size, file);
    fclose(file);
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete [] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture_id;
}















