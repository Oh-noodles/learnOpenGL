#ifndef MODEL_H
#define MODEL_H

#include "assimp/material.h"
#include "glm/detail/type_vec.hpp"
#include <cstring>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learn/mesh.hpp>
#include <learn/shader_s.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

/* #define STB_IMAGE_IMPLEMENTATION */
#include <learn/stb_image.h>
       //
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model {
  public:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    glm::vec3 position;

    Model(string const &path, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) {
      loadModel(path);
      this->position = position;
    }

    void draw(Shader &shader) {
      for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
      }
    }
  private:
    void loadModel(string const &path) {
      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
      }
      directory = path.substr(0, path.find_last_of('/'));

      processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene) {
      for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
      }
      for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
      }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
      vector<Vertex> vertices;
      vector<unsigned int> indices;
      vector<Texture> textures;

      // walk through vertices
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // declare a placeholder vector since assimp uses its own vector class
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals()) {
          vector.x = mesh->mNormals[i].x;
          vector.y = mesh->mNormals[i].y;
          vector.z = mesh->mNormals[i].z;
          vertex.normal = vector;
        }
        // texture coords
        if (mesh->mTextureCoords[0]) {
          glm::vec2 vec;
          vec.x = mesh->mTextureCoords[0][i].x;
          vec.y = mesh->mTextureCoords[0][i].y;
          vertex.texCoords = vec;
        } else {
          vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
      }

      // walk through faces
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
          indices.push_back(face.mIndices[j]);
        }
      }

      // process materials
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
      // diffuse maps
      vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      // specular maps
      vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      return Mesh(vertices, indices, textures);
    }

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
      vector<Texture> textures;
      for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
          if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
            textures.push_back(textures_loaded[j]);
            skip = true;
            break;
          }
        }
        if (!skip) {
          Texture texture;
          texture.id = textureFromFile(str.C_Str(), this->directory);
          texture.type = typeName;
          texture.path = str.C_Str();
          textures.push_back(texture);
          textures_loaded.push_back(texture);
        }
      }
      return textures;
    }

    unsigned int textureFromFile(const char *path, const string &directory) {
      string filename = string(path);
      filename = directory + '/' + filename;

      unsigned int textureID;
      glGenTextures(1, &textureID);

      int width, height, nrComponents;
      unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
      if (data) {
        GLenum format;
        switch (nrComponents) {
          case 1:
            format = GL_RED;
            break;
          case 3:
            format = GL_RGB;
            break;
          case 4:
            format = GL_RGBA;
            break;
          default:
            break;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
      }
      else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
      }

      return textureID;
    }
};

#endif
