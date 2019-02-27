#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    enum cameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera(glm::vec3 cam_pos, glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f), float cam_yaw = -90.0f, float cam_pitch = 0.0f, glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
private:
    void UpdateCameraVectors();
};
#endif