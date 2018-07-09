#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <Sources/Common/Shader.h>
#include <vector>
#include <Sources/Engine/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


unsigned int TextureFromFile(const char * path, const std::string& directory, bool gamma = false);

class Model
{
public:
    Model(const std::string& path);
    void draw(Shader * pShader);
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    void loadModel(const std::string &path);
    void processNode(aiNode * node, const aiScene * scene);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H
