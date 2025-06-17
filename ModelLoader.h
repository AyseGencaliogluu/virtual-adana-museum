#ifndef MODELLOADER_H
#define MODELLOADER_H

// Assimp headers for model loading
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Standard and GLM libraries
#include <vector>
#include <string>
#include <glm/glm.hpp>

// Structure to store vertex data: position and normal
struct Vertex {
    glm::vec3 Position;  // 3D position of the vertex
    glm::vec3 Normal;    // Normal vector for lighting
};

// Class to load and render a 3D model
class ModelLoader {
public:
    std::vector<Vertex> vertices;  // List of vertices extracted from the model

    // Constructor: loads a model from the given file path
    ModelLoader(const std::string& path);

    // Draws the loaded model using OpenGL
    void drawModel();

private:
    unsigned int VAO, VBO;  // OpenGL buffers: Vertex Array Object and Vertex Buffer Object

    // Extracts vertices and normals from the given mesh
    void processMesh(aiMesh* mesh);

    // Sets up the OpenGL VAO and VBO for rendering
    void setupBuffers();
};

#endif