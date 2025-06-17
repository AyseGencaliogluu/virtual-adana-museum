#include "Primitives.h"
#include <vector>
#include <cmath>

// Define PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Static VAO and VBO for primitive reuse
static unsigned int cubeVAO = 0, cubeVBO = 0;
static unsigned int cylVAO = 0, cylVBO = 0;
static unsigned int sphereVAO = 0, sphereVBO = 0;

// Function to draw a cube using a VAO/VBO setup
void drawCube(Shader& shader, const glm::mat4& transform) {
    if (cubeVAO == 0) {
        // Vertex data for a unit cube centered at the origin
        float vertices[] = {
            // 6 faces with 2 triangles each, total 36 vertices
            -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
             0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
            -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
            -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
             0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
            -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
             0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
        };

        // Generate VAO and VBO for cube
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Set model matrix in shader
    shader.setMat4("model", transform);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Function to draw a cylinder using triangle strip logic
void drawCylinder(Shader& shader, const glm::mat4& transform) {
    const int segments = 36;
    if (cylVAO == 0) {
        std::vector<float> vertices;

        // Generate side faces of the cylinder
        for (int i = 0; i < segments; ++i) {
            float theta = (float)i / segments * 2.0f * M_PI;
            float nextTheta = (float)(i + 1) / segments * 2.0f * M_PI;
            float x1 = cos(theta), z1 = sin(theta);
            float x2 = cos(nextTheta), z2 = sin(nextTheta);

            // Add two triangles per segment
            vertices.insert(vertices.end(), {
                x1, -0.5f, z1,  x2, -0.5f, z2,  x2, 0.5f, z2,
                x2, 0.5f, z2,  x1, 0.5f, z1,  x1, -0.5f, z1
                });
        }

        // Generate VAO/VBO
        glGenVertexArrays(1, &cylVAO);
        glGenBuffers(1, &cylVBO);
        glBindVertexArray(cylVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cylVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    shader.setMat4("model", transform);
    glBindVertexArray(cylVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36 * 6); // 6 vertices per segment
}

// Function to draw a sphere made of latitude and longitude segments
void drawSphere(Shader& shader, const glm::mat4& transform) {
    if (sphereVAO == 0) {
        std::vector<float> vertices;
        const unsigned int X_SEGMENTS = 17;
        const unsigned int Y_SEGMENTS = 17;

        // Generate vertex positions for the sphere
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                float xSegment = (float)x / X_SEGMENTS;
                float ySegment = (float)y / Y_SEGMENTS;
                float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
                float yPos = cos(ySegment * M_PI);
                float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);
            }
        }

        // Generate VAO/VBO
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    shader.setMat4("model", transform);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_POINTS, 0, (16 + 1) * (16 + 1)); // Drawing as points
}

// Function to draw a humanoid robot with animation
void drawHumanoidRobot(Shader& shader, glm::vec3 robotPos, float time, bool isScanning, float scanAngle) {
    // Animated angles for arms and legs
    float armAngle = sin(time * 0.5f) * glm::radians(30.0f);
    float legAngle = sin(time * 0.5f) * glm::radians(30.0f);

    // Torso (cube)
    glm::mat4 torso = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(0.0f, 1.2f, 0.0f));
    torso = glm::scale(torso, glm::vec3(0.25f, 1.2f, 0.2f));
    shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
    drawCube(shader, torso);

    // Head (sphere)
    glm::mat4 head = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(0.0f, 2.1f, 0.0f));
    head = glm::scale(head, glm::vec3(0.2f));
    shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
    drawSphere(shader, head);

    // Left arm (cylinder, animated)
    glm::mat4 leftArm = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(-0.3f, 1.3f, 0.0f));
    leftArm = glm::rotate(leftArm, armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    leftArm = glm::scale(leftArm, glm::vec3(0.1f, 0.8f, 0.1f));
    drawCylinder(shader, leftArm);

    // Right arm (cylinder, rotates if scanning)
    glm::mat4 rightArm = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(0.3f, 1.3f, 0.0f));
    if (isScanning) {
        rightArm = glm::rotate(rightArm, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    rightArm = glm::scale(rightArm, glm::vec3(0.1f, 0.8f, 0.1f));
    shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
    drawCylinder(shader, rightArm);

    // Left leg (cube, animated)
    glm::mat4 leftLeg = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(-0.2f, 0.4f, 0.0f));
    leftLeg = glm::rotate(leftLeg, legAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    leftLeg = glm::scale(leftLeg, glm::vec3(0.15f, 1.2f, 0.15f));
    drawCube(shader, leftLeg);

    // Right leg (cube, opposite angle)
    glm::mat4 rightLeg = glm::translate(glm::mat4(1.0f), robotPos + glm::vec3(0.2f, 0.4f, 0.0f));
    rightLeg = glm::rotate(rightLeg, -legAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    rightLeg = glm::scale(rightLeg, glm::vec3(0.15f, 1.2f, 0.15f));
    drawCube(shader, rightLeg);
}