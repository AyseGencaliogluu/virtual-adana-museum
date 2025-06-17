#version 330 core

// Output color of the fragment
out vec4 FragColor;

// Inputs from the vertex shader
in vec3 FragPos;   // Fragment position in world space
in vec3 Normal;    // Normal vector at the fragment

// Uniforms (passed in from the CPU program)
uniform vec3 lightPos;     // Position of the light source
uniform vec3 lightColor;   // Color of the light
uniform vec3 objectColor;  // Base color of the object

void main() {
    // ----- Ambient Lighting -----
    // A small constant light that simulates global illumination
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // ----- Diffuse Lighting -----
    // Light that depends on angle between light direction and surface normal
    vec3 norm = normalize(Normal);                       // Normalize the surface normal
    vec3 lightDir = normalize(lightPos - FragPos);       // Direction from fragment to light
    float diff = max(dot(norm, lightDir), 0.0);          // Lambert's cosine law
    vec3 diffuse = diff * lightColor;                    // Final diffuse component

    // Combine ambient and diffuse lighting
    vec3 result = (ambient + diffuse) * objectColor;

    // Set final fragment color
    FragColor = vec4(result, 1.0); // Alpha = 1.0 (fully opaque)
}
