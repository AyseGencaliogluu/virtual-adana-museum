#include "Room.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ModelLoader.h"
#include "Primitives.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Room::Room() {
    shader = new Shader("vertex_shader.glsl", "fragment_shader.glsl");
    setupFloor();
    setupWall();
    setupPlinth();
    setupModels();

    // Initial robot position and target object coordinates
    robotPosition = glm::vec3(0.0f, 0.0f, 0.0f);// Starting position

    currentTargetIndex = 0;
    autoMode = true;

    objectPositions = {
        {3.0f, 0.0f, 3.0f},      // model1.obj
        {-3.5f, 0.0f, -1.0f},    // model2.obj
        {0.0f, 1.0f, -6.0f},     // model3.obj
        {4.0f, 0.0f, -4.0f},     // model5.obj
        {-3.5f, 0.0f, 2.5f}      // model4.obj
    };

}

Room::~Room() {
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteVertexArrays(1, &wallVAO);
    glDeleteBuffers(1, &wallVBO);
    glDeleteVertexArrays(1, &plinthVAO);
    glDeleteBuffers(1, &plinthVBO);
    delete shader;
    for (auto m : models) delete m; // Delete all models

}

void Room::setupFloor() {
    float vertices[] = {
        // zemin düzlemi (10x10)
        -5.0f, 0.0f,  5.0f,   0, 1, 0,
         5.0f, 0.0f,  5.0f,   0, 1, 0,
         5.0f, 0.0f, -5.0f,   0, 1, 0,
        -5.0f, 0.0f, -5.0f,   0, 1, 0
    };
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Room::setupWall() {
    float vertices[] = {
        // Düz dikdörtgen duvar
        -5.0f, 0.0f, 0.0f,  0, 0, 1,
         5.0f, 0.0f, 0.0f,  0, 0, 1,
         5.0f, 5.0f, 0.0f,  0, 0, 1,
        -5.0f, 5.0f, 0.0f,  0, 0, 1
    };
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &wallVBO);
    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Room::setupPlinth() {
    // Top face of a small cube using 6 vertices

    float vertices[] = {
        -0.5f, 0.0f,  0.5f,   0, 1, 0,
         0.5f, 0.0f,  0.5f,   0, 1, 0,
         0.5f, 0.0f, -0.5f,   0, 1, 0,
         0.5f, 0.0f, -0.5f,   0, 1, 0,
        -0.5f, 0.0f, -0.5f,   0, 1, 0,
        -0.5f, 0.0f,  0.5f,   0, 1, 0,
    };
    glGenVertexArrays(1, &plinthVAO);
    glGenBuffers(1, &plinthVBO);
    glBindVertexArray(plinthVAO);
    glBindBuffer(GL_ARRAY_BUFFER, plinthVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Room::setupModels() {
    // model1.obj
    models.push_back(new ModelLoader("models/model1.obj"));
    // model2.obj
    models.push_back(new ModelLoader("models/model2.obj"));
    // model3.obj
    models.push_back(new ModelLoader("models/model3.obj"));
    // model5.obj
    models.push_back(new ModelLoader("models/model5.obj"));
    // model4.obj
    models.push_back(new ModelLoader("models/model4.obj"));

}

void Room::drawMuseumRoom(const glm::mat4& view, const glm::mat4& projection) {
    shader->use();

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    shader->setVec3("lightPos", glm::vec3(0.0f, 4.5f, 0.0f));
    shader->setVec3("lightColor", glm::vec3(1.0f));

    // Floor
    shader->setVec3("objectColor", glm::vec3(0.6f, 0.6f, 0.6f)); // Walls are light gray

    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // 4 Walls
    glm::vec3 wallColors[4] = {
        {0.6f, 0.6f, 0.6f}, // Back

        {0.4f, 0.4f, 0.4f}, // Front

        {0.5f, 0.5f, 0.5f}, // Left

        {0.7f, 0.7f, 0.7f}  // Right
    };

    glm::vec3 positions[4] = {
        {0, 0, -5}, {0, 0, 5}, {-5, 0, 0}, {5, 0, 0}
    };

    glm::vec3 rotations[4] = {
        {0, 0, 0}, {0, 180, 0}, {0, -90, 0}, {0, 90, 0}
    };

    for (int i = 0; i < 4; i++) {
        model = glm::translate(glm::mat4(1.0f), positions[i]);
        model = glm::rotate(model, glm::radians(rotations[i].y), glm::vec3(0, 1, 0));

        if (i == 2 || i == 3) { // Left or Right wall

            model = glm::scale(model, glm::vec3(5.0f, 2.0f, 20.0f));
        }
        else {
            model = glm::scale(model, glm::vec3(10.0f, 1.0f, 6.0f));
        }

        shader->setVec3("objectColor", wallColors[i]);
        shader->setMat4("model", model);
        glBindVertexArray(wallVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // Draw all models
    for (size_t i = 0; i < models.size(); ++i) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        if (i == 0) { // model1.obj
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 3.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
            shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));

            if (i == scannedObjectIndex && isScanning) {
                shader->setVec3("objectColor", glm::vec3(0.7f, 1.0f, 0.7f)); // coloring
                modelMatrix = glm::rotate(modelMatrix, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f));// Apply rotation

            }
            else {
                shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));// Default color

            }

        }
        else if (i == 1) { // model2.obj
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.5f, 0.0f, -1.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
            shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));

            if (i == scannedObjectIndex && isScanning) {
                shader->setVec3("objectColor", glm::vec3(0.7f, 1.0f, 0.7f)); // coloring
                modelMatrix = glm::rotate(modelMatrix, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f));// Apply rotation
            }
            else {
                shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));// Default color
            }
        }
        else if (i == 2) { // model3.obj
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, -6.0f));// Further to the right and forward

            modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
            shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));

            if (i == scannedObjectIndex && isScanning) {
                shader->setVec3("objectColor", glm::vec3(0.7f, 1.0f, 0.7f)); //  coloring
                modelMatrix = glm::rotate(modelMatrix, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply rotation
            }
            else {
                shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f)); // Default color
            }
        }
        else if (i == 3) { // model5.obj
            modelMatrix = glm::translate(modelMatrix, glm::vec3(4.0f, 0.0f, -4.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f, 1.8f, 1.8f));
            shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));

            if (i == scannedObjectIndex && isScanning) {
                shader->setVec3("objectColor", glm::vec3(0.7f, 1.0f, 0.7f)); //  coloring
                modelMatrix = glm::rotate(modelMatrix, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f)); //Apply rotation
            }
            else {
                shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f)); // Default color
            }
        }
        else if (i == 4) { // model4.obj
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.5f, 0.0f, 2.5f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));// Rotated 90 degrees clockwise

            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.4f, 1.4f, 1.4f));
            shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f));

            if (i == scannedObjectIndex && isScanning) {
                shader->setVec3("objectColor", glm::vec3(0.7f, 1.0f, 0.7f)); // coloring
                modelMatrix = glm::rotate(modelMatrix, glm::radians(scanAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply rotation
            }
            else {
                shader->setVec3("objectColor", glm::vec3(1.0f, 0.95f, 0.7f)); // Default color
            }
        }

        shader->setMat4("model", modelMatrix);
        models[i]->drawModel();
    }
    drawHumanoidRobot(*shader, robotPosition, glfwGetTime(), isScanning, scanAngle);

    // --- IMGUI CONTROL PANEL ---
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.5f)); // Background

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));   // Text color


    ImGui::Begin("Robot Control Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowFontScale(2.0f);

    // Toggle between automatic/manual mode
    if (ImGui::Checkbox("Automatic Mode", &autoMode)) {
        goToTargetManually = false;  // turn off manual flag when switching to auto
    }

    ImGui::Separator();

    // Manual object targeting
    if (!autoMode) {
        if (ImGui::Button("Go to Object 1")) {
            currentTargetIndex = 0;
            goToTargetManually = true;
        }
        if (ImGui::Button("Go to Object 2")) {
            currentTargetIndex = 1;
            goToTargetManually = true;
        }
        if (ImGui::Button("Go to Object 3")) {
            currentTargetIndex = 2;
            goToTargetManually = true;
        }
        if (ImGui::Button("Go to Object 4")) {
            currentTargetIndex = 3;
            goToTargetManually = true;
        }
        if (ImGui::Button("Go to Object 5")) {
            currentTargetIndex = 4;
            goToTargetManually = true;
        }
    }

    if (showScanPopup && scannedObjectIndex >= 0) {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 500, 20), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(500, 350));

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.5f));  // Transparent dark blue

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Golden yellow text

        ImGui::Begin("Scanned Object", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        ImGui::SetWindowFontScale(2.5f);

        ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.3f, 1.0f), "Object %d has been scanned!", scannedObjectIndex + 1); // Golden header

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextWrapped(" ");

        if (scannedObjectIndex == 0)
            ImGui::TextWrapped("Sarcophagus of Achilles\nRoman Period (2nd Century AD)\nA marble tomb depicting the life of Achilles. Dated to the Late Antonine Period.");

        else if (scannedObjectIndex == 1)
            ImGui::TextWrapped("Man Statue\nRoman Period (1st Century AD)\nA bronze male figure discovered in Karataþ, Adana.");

        else if (scannedObjectIndex == 2)
            ImGui::TextWrapped("Statue of Tarhunda with Chariot\nLate Hittite Period (8th Century BC)\nA monumental sculpture made of basalt and limestone, depicting the Storm God Tarhunda on his chariot. It was discovered in the Yüregir district of Adana Province.");

        else if (scannedObjectIndex == 3)
            ImGui::TextWrapped("Tombstone with Figures\nRoman Period (2nd and 3rd Century AD)\nA stone tombstone decorated with human or symbolic figures.");

        else if (scannedObjectIndex == 4)
            ImGui::TextWrapped("Sarcophagus\nRoman Period (3rd Century AD)\nAn ancient marble sarcophagus from the Roman era.");

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2); // Text & WindowBg
    }

    ImGui::End();
    ImGui::PopStyleColor(2);
}

void Room::update(float deltaTime) {
    // Do nothing if the target list is finished
    if (currentTargetIndex >= objectPositions.size()) return;

    // Exit if no target is set in manual mode
    if (!autoMode && !goToTargetManually) return;

    // Target position and direction
    glm::vec3 target = objectPositions[currentTargetIndex];
    glm::vec3 direction = glm::normalize(target - robotPosition);
    float distance = glm::length(target - robotPosition);

    float speed = glm::clamp(distance * 1.5f, 0.3f, 2.0f);

    // Robot should move until it gets close to the target
    if (!isScanning && distance > 0.4f) {
        robotPosition += direction * speed * deltaTime;
    }
    // --- If the target is reached, start scanning (not the popup yet, just the rotation)
    else if (!isScanning && distance <= 0.4f && scannedObjectIndex != currentTargetIndex) {
        isScanning = true;
        scanAngle = 0.0f;
        scanTimer = 3.0f;
        scannedObjectIndex = currentTargetIndex;  // Only when arriving for the first time

        showScanPopup = false;

        // SCANNING SOUND (on start)
        PlaySound(TEXT("scan_loop.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    // Execute scanning process
    if (isScanning) {
        scanAngle += 120.0f * deltaTime;  // Rotates at 120 degrees per second

        scanTimer -= deltaTime;

        if (scanTimer <= 0.0f) {
            isScanning = false;
            showScanPopup = true;   // Show popup when scanning is complete

            popupTimer = 3.0f;

            // STOP SCANNING SOUND
            PlaySound(NULL, 0, 0);

            // FINISH SOUND
            PlaySound(TEXT("scan_done.wav"), NULL, SND_FILENAME | SND_ASYNC);

            if (autoMode) currentTargetIndex++;
            else goToTargetManually = false;
        }
    }

    if (popupTimer > 0.0f) {
        popupTimer -= deltaTime;
        if (popupTimer <= 0.0f) {
            showScanPopup = false;
        }
    }

}