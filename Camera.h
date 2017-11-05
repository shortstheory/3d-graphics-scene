#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include <utility>

class Camera {
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float fov;
    float lastX;
    float lastY;
    float MAX_FOV;

    GLfloat x_min;
    GLfloat x_max;
    GLfloat y_min;
    GLfloat y_max;
    GLfloat z_min;
    GLfloat z_max;

    bool firstMouse;

public:
    Camera(int screenHeight, int screenWidth)
    {
        lastX = screenWidth/2.0f;
        lastY = screenHeight/2.0f;
        firstMouse = true;
        yaw = -90.0f;
        pitch =  0.0f;
        MAX_FOV = 90;
        fov = MAX_FOV;
        cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        this->x_min = -100;
        this->x_max = 100;
        this->y_min = -100;
        this->y_max = 100;
        this->z_min = -100;
        this->z_max = 100;
    }

    Camera(int screenHeight, int screenWidth, GLfloat x_min, GLfloat x_max, GLfloat y_min, GLfloat y_max, GLfloat z_min, GLfloat z_max)
    {
        lastX = screenWidth/2.0f;
        lastY = screenHeight/2.0f;
        firstMouse = true;
        yaw = -90.0f;
        pitch =  0.0f;
        MAX_FOV = 90;
        fov = MAX_FOV;
        cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
        this->z_min = z_min;
        this->z_max = z_max;
    }

    glm::mat4 getCameraView()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    float getFOV()
    {
        return fov;
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (fov >= 1.0f && fov <= MAX_FOV)
            fov -= yoffset;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= MAX_FOV)
            fov = MAX_FOV;
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

    void processInput(GLFWwindow *window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = 2.0 * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        if (cameraPos.x < x_min)
            cameraPos.x = x_min;
        if (cameraPos.x > x_max)
            cameraPos.x = x_max;
        if (cameraPos.y < y_min)
            cameraPos.y = y_min;
        if (cameraPos.y > y_max)
            cameraPos.y = y_max;
        if (cameraPos.z < z_min)
            cameraPos.z = z_min;
        if (cameraPos.z > z_max)
            cameraPos.z = z_max;
    }
};
