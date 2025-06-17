#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Room.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Callback function to adjust the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow* window) {
    // Close the window when ESC key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configure GLFW to use OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Compatibility for macOS
#endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Virtual Museum Assignment", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Setup ImGui context and initialize backends
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Set the callback function for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set initial OpenGL viewport size
    glViewport(0, 0, 800, 600);

    float lastFrame = 0.0f;

    // Create a Room object which manages the scene
    Room* room;
    room = new Room();

    // Main application loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle user input
        processInput(window);

        // Update the room (e.g., animations, robot movement, etc.)
        room->update(deltaTime);

        // Clear the screen with a dark gray color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the camera view matrix
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 3.0f, 10.0f),    // Camera position (slightly elevated and pulled back)
            glm::vec3(0.0f, 2.0f, 0.0f),     // Look at the center of the room
            glm::vec3(0.0f, 1.0f, 0.0f)      // Up vector
        );

        // Set up the perspective projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(55.0f), 800.f / 600.f, 0.1f, 100.0f);

        // Render the room with current view and projection
        room->drawMuseumRoom(view, projection);

        // Render ImGui UI elements
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll window events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Terminate GLFW and clean up
    glfwTerminate();
    return 0;
}
