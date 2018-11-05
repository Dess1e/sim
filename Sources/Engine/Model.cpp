#include "Sources/Engine/Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <ThirdParty/stb/stb_image.h>
#include <Sources/Engine/Engine.h>

Model::Model(const std::string &path)
{
    this->loadModel(path);
}

void Model::draw(Shader *pShader)
{
    if (this->meshes.empty())
    {
        Engine::getEngine()->debugPrint(Engine::DebugLevel::Error,
                                        "Model from dir " + this->directory + " have zero meshes");
        return;

    }
    for (auto& mesh : this->meshes)
    {
        mesh.draw(pShader);
    }
}

void Model::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //create exception and throw it
        printf("assimp error: %s\n", importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
   std::vector<Vertex> vertices;
   std::vector<Texture> textures;
   std::vector<GLuint> indices;
   for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
   {
       Vertex vertex;
       glm::vec3 pos;
       pos.x = mesh->mVertices[i].x;
       pos.y = mesh->mVertices[i].y;
       pos.z = mesh->mVertices[i].z;
       vertex.position = pos;
       glm::vec3 normal;
       normal.x = mesh->mNormals[i].x;
       normal.y = mesh->mNormals[i].y;
       normal.z = mesh->mNormals[i].z;
       vertex.normal = normal;
       if (mesh->mTextureCoords[0])
       {
           glm::vec2 coord;
           coord.x = mesh->mTextureCoords[0][i].x;
           coord.y = mesh->mTextureCoords[0][i].y;
           vertex.tex_coords = coord;
       }
       else
           vertex.tex_coords = glm::vec2(0.0f, 0.0f);
       if (mesh->mTangents)
       {
           glm::vec3 tangent;
           tangent.x = mesh->mTangents[i].x;
           tangent.y = mesh->mTangents[i].y;
           tangent.z = mesh->mTangents[i].z;
           vertex.tangent = tangent;
       }
       else
           vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
       if (mesh->mBitangents)
       {
           glm::vec3 bitangent;
           bitangent.x = mesh->mBitangents[i].x;
           bitangent.y = mesh->mBitangents[i].y;
           bitangent.z = mesh->mBitangents[i].z;
           vertex.bitangent = bitangent;
       }
       else
           vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
       vertices.push_back(vertex);
   }
   for (unsigned int i = 0; i < mesh->mNumFaces; ++i )
   {
       aiFace face = mesh->mFaces[i];
       for (unsigned int j = 0; j < face.mNumIndices; ++j)
           indices.push_back(face.mIndices[j]);
   }
   aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
   std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
   textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

   std::vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
   textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

   std::vector<Texture> normal_maps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
   textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

   std::vector<Texture> height_maps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
   textures.insert(textures.end(), height_maps.begin(), height_maps.end());

   return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); ++j)
        {
            if (strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char * path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nrComponents;
    unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = GL_RGB;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    }
    else
    {
        //throw error texture failed to load at path
        printf("texture failed to load at path %s", path);
        stbi_image_free(data);
    }
    return texture_id;
}
















