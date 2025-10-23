#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "header.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.5f;
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float ZOOM = 60.0f;


class Camera
{
public:
    const float movementSpeed;
    const float sensitivity;
    bool ableToMove = true;

    int initialized = 0;

    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;

    //DELTA TIME
    float deltaTime = -1.0f;
    float lastFrame = 0.0f;

    //mouse data
    bool firstMouse = true;
    float mLastX = -1;
    float mLastY = -1;
    float Yaw = -90.0f, Pitch = 0.0f, Roll = 0.0f;
    float Zoom;

    //perspective and orthographic!
    glm::mat4 projection = glm::mat4(1.0f);
    bool perspective = true;

    // constructor reads and builds the shader
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = UP, float yaw = YAW,
            float pitch = PITCH);

    //Necessarry AdjacentCalls
    void setInputMode(GLFWwindow* window, float resolutionx, float resolutiony);
    void calculateDeltaTime();

    glm::mat4 GetViewMatrix();
    void movePredefinedCircle(const float radius = 10.0f);

    //PROCESS INPUT
    void processInput(GLFWwindow* window);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        //--------

    //PERSPECTIVE and ORTHOGRAPHIC
    void togglePerspective();
    glm::mat4 Camera::getProjection(float FOV, glm::vec2 screenSize, float nearPlane, float farPlane);
    glm::mat4 Camera::getProjection(float FOV, glm::vec2 screenSize, float nearPlane, float farPlane, float leftBound, float rightBound, float bottomBound, float topBound);
    glm::mat4 Camera::getProjection(glm::vec2 screenSize, float nearPlane, float farPlane);
    glm::mat4 Camera::getProjection(glm::vec2 screenSize, float nearPlane, float farPlane, float leftBound, float rightBound, float bottomBound, float topBound);

    void dynamicCursorLocking(GLFWwindow* window, int GLFW_MOUSE_BUTTON);

    private:
    // utility uniform functions
    void refreshVectors();
    void ErrorHandling();

};

#endif