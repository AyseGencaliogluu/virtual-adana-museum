#ifndef PRIMITIVES_H
#define PRIMITIVES_H

// OpenGL loader and GLM for transformations
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Custom shader class
#include "Shader.h"

// Standard libraries
#include <vector>
#include <cmath>

// Draws a cube with the given shader and transformation matrix
void drawCube(Shader& shader, const glm::mat4& transform);

// Draws a cylinder (approximated by triangle segments)
void drawCylinder(Shader& shader, const glm::mat4& transform);

// Draws a sphere using latitude and longitude segments
void drawSphere(Shader& shader, const glm::mat4& transform);

// Draws a humanoid robot composed of cubes, spheres, and cylinders
// - robotPos: position in the scene
// - time: used for animation (arms/legs movement)
// - isScanning: if true, enables scanning animation for the right arm
// - scanAngle: rotation angle of the scanning arm
void drawHumanoidRobot(Shader& shader, glm::vec3 robotPos, float time, bool isScanning = false, float scanAngle = 0.0f);

#endif