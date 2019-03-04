#ifndef MY_MODEL_H
#define MY_MODEL_H

#include "mesh.h"
#include "shaderprogram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(char* path);
    void draw(Shaderprogram& shaderprogram);
protected:
private:
    std::vector<Mesh> meshes;
    std::string directory;
    /*  Functions   */
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif // MY_MODEL_H