#ifndef ROOM_H
#define ROOM_H

// OpenGL and GLM includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>           // For storing multiple models or positions

#include "Shader.h"
#include "ModelLoader.h"

// Room class handles the rendering and logic of the virtual museum scene
class Room {
public:
    Room();                  // Constructor
    ~Room();                 // Destructor

    // Renders the museum room with camera view and projection
    void drawMuseumRoom(const glm::mat4& view, const glm::mat4& projection);

    // Updates the scene state over time (e.g., robot movement)
    void update(float deltaTime);

private:
    // Floor geometry
    unsigned int planeVAO, planeVBO;

    // Wall geometry
    unsigned int wallVAO, wallVBO;

    // Plinth 
    unsigned int plinthVAO, plinthVBO;

    // Main shader used in the room
    Shader* shader;

    // Popup display state for scanned objects
    bool showScanPopup = false;
    int scannedObjectIndex = -1;
    float popupTimer = 0.0f;

    // List of models loaded and displayed in the museum
    std::vector<ModelLoader*> models;

    // Room setup methods
    void setupFloor();       // Initializes floor geometry
    void setupWall();        // Initializes walls
    void setupPlinth();      // Initializes object display stands
    void setupModels();      // Loads 3D object models

    // Robot-related state and navigation
    std::vector<glm::vec3> objectPositions;  // Positions of models for robot to visit
    glm::vec3 robotPosition;                 // Current robot position
    int currentTargetIndex;                 // Index of the object robot is moving toward
    bool autoMode;                          // If true, robot navigates automatically
    bool goToTargetManually = false;        // Manual override for movement

    // Scanning logic
    float scanAngle = 0.0f;
    bool isScanning = false;
    float scanTimer = 0.0f;

    // State for tracking if all objects have been scanned
    bool allScanned = false;
    float allScannedTimer = 0.0f;
};

#endif