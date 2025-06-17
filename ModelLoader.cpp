#include "ModelLoader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>

// Constructor: Loads the model from the given file path
ModelLoader::ModelLoader(const std::string& path) {
    Assimp::Importer importer;

    // Read the model file with triangulation and normal generation
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    // Error checking: ensure the scene was loaded correctly
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    // For simplicity, only the first mesh is processed
    aiMesh* mesh = scene->mMeshes[0];

    // Process the mesh to extract vertex data
    processMesh(mesh);

    // Set up OpenGL buffers (VAO, VBO)
    setupBuffers();
}

// Extracts vertex positions and normals from the mesh
void ModelLoader::processMesh(aiMesh* mesh) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Set vertex position
        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Set vertex normal
        vertex.Normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        // Add the vertex to the list
        vertices.push_back(vertex);
    }
}

// Sets up the Vertex Array Object and Vertex Buffer Object
void ModelLoader::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // Bind and load vertex data into the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Set vertex attribute for position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Set vertex attribute for normal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    // Unbind the VAO to avoid accidental changes
    glBindVertexArray(0);
}

// Draws the model using glDrawArrays
void ModelLoader::drawModel() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Draw all vertices as triangles
    glBindVertexArray(0);
}