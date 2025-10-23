#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
    : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), sensitivity(SENSITIVITY), Zoom(ZOOM)
{
    cameraPosition = position;
    cameraUp = up;
    Yaw = yaw;
    Pitch = pitch;

    initialized++;

    refreshVectors();
}

void Camera::setInputMode(GLFWwindow* window, float resolutionx, float resolutiony)
{
    glm::vec2 resolution = glm::vec2(resolutionx, resolutiony);

    //had to do a bit of research to figure out why my mouse_callback was giving error, apparently the fucntion needed to be static
    //which as a static i then needed to grab a reference to the camera class from inside of it, and this facilitated that by storing the pointer
    //and retrieving it through opengl, and it somehow works now! The only thing i dont totally understand is how the whole WindowUserPoitner interacts with all
    //of opengl
    //this is all just because i wanted to call these things from inside of the camera class
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, Camera::mouse_callback);
    glfwSetScrollCallback(window, Camera::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mLastX = resolution.x / 2.0f;
    mLastY = resolution.y / 2.0f;

    initialized++;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Camera::movePredefinedCircle(const float radius)
{
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), cameraUp);
}

//PROCESS INPUT
void Camera::processInput(GLFWwindow* window)
{
    if (initialized < 3)
        ErrorHandling();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(movementSpeed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
       cameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        togglePerspective();

    if (!ableToMove)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
       cameraPosition += cameraSpeed * cameraUp;  // Move up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
       cameraPosition -= cameraSpeed * cameraUp;  // Move down

}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //had to do a bit of research to figure out why my mouse_callback was giving error, apparently the fucntion needed to be static
    //which as a static i then needed to grab a reference to the camera class from inside of it, and this facilitated that by storing the pointer
    //and retrieving it through opengl, and it somehow works now! The only thing i dont totally understand is how the whole WindowUserPoitner interacts with all
    //of opengl
    //this is all just because i wanted to call these things from inside of the camera class

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (!camera->ableToMove)
        return;

    if (camera->firstMouse)
    {
        camera->mLastX = xpos;
        camera->mLastY = ypos;
        camera->firstMouse = false;
    }

    float xoffset = xpos - camera->mLastX;
    float yoffset = camera->mLastY - ypos;
    camera->mLastX = xpos;
    camera->mLastY = ypos;

    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (camera->Pitch > 89.0f)
        camera->Pitch = 89.0f;
    if (camera->Pitch < -89.0f)
        camera->Pitch = -89.0f;

    camera->refreshVectors();
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //had to do a bit of research to figure out why my mouse_callback was giving error, apparently the fucntion needed to be static
    //which as a static i then needed to grab a reference to the camera class from inside of it, and this facilitated that by storing the pointer
    //and retrieving it through opengl, and it somehow works now! The only thing i dont totally understand is how the whole WindowUserPoitner interacts with all
    //of opengl
    //this is all just because i wanted to call these things from inside of the camera class

   Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

   camera->Zoom -= (float)yoffset;
   if (camera->Zoom < 1.0f)
      camera->Zoom = 1.0f;
   if (camera->Zoom > 120.0f)
      camera->Zoom = 120.0f;
}


//--------

void Camera::calculateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (initialized < 3)
        initialized++;
}

// utility uniform functions
void Camera::refreshVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    cameraFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    cameraRight = glm::normalize(glm::cross(cameraFront, UP));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::ErrorHandling()
{
    std::cout << "ERROR::CAMERA::ALL NECESSARY FUNCTIONS NOT INIALIZED/USED" << std::endl;
    if (mLastX < 0 || mLastY < 0)
        std::cout << "ERROR.CONT::CAMERA::SET_INPUT_MODE NOT CALLED?";
    if (deltaTime < 0)
        std::cout << "ERROR.CONT::CAMERA::CALCULATE_DELTA_TIME NOT CALLED?";
}

void Camera::togglePerspective()
{
    perspective = !perspective;
}

//default to being only perspective, but if overloads provided then can toggle to orthographic live
glm::mat4 Camera::getProjection(float FOV, glm::vec2 screenSize, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(FOV), (float)screenSize.x / (float)screenSize.y, nearPlane, farPlane);   
}
glm::mat4 Camera::getProjection(float FOV, glm::vec2 screenSize, float nearPlane, float farPlane, float leftBound, float rightBound, float bottomBound, float topBound)
{
    if (perspective)
    {
        return glm::perspective(glm::radians(FOV), (float)screenSize.x / (float)screenSize.y, nearPlane, farPlane);
    }
    else
    {
        return glm::ortho(leftBound, rightBound, bottomBound, topBound, nearPlane, farPlane);
    }
}
//without a dynamic Zoom
glm::mat4 Camera::getProjection(glm::vec2 screenSize, float nearPlane, float farPlane)
{
   return glm::perspective(glm::radians(Zoom), (float)screenSize.x / (float)screenSize.y, nearPlane, farPlane);
}
glm::mat4 Camera::getProjection(glm::vec2 screenSize, float nearPlane, float farPlane, float leftBound, float rightBound, float bottomBound, float topBound)
{
   if (perspective)
   {
      return glm::perspective(glm::radians(Zoom), (float)screenSize.x / (float)screenSize.y, nearPlane, farPlane);
   }
   else
   {
      return glm::ortho(leftBound, rightBound, bottomBound, topBound, nearPlane, farPlane);
   }
}

//cursor locking / unlocking!
void Camera::dynamicCursorLocking(GLFWwindow* window, int GLFW_MOUSE_BUTTON)
{
    //Mouse Cursor/Lock Unlock!
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ableToMove = true;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ableToMove = false;
        firstMouse = true;
    }
}
