#include "extern_includes.hpp"
#include "defines.hpp"
#include "camera.hpp"
#include "movement.hpp"

float last_mouse_x = 0.0f;
float last_mouse_y = 0.0f;
float yaw = 90.0f;
float pitch = 0.0f;
bool first_mouse = true;

void process_keyboard(GLFWwindow *window, float delta_time) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    bool front = false, back = false, left = false, right = false, up = false, down = false;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        front = true;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        back = true;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        left = true;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        right = true;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        up = true;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        down = true;
    }

    movement_process(front, back, left, right, up, down, delta_time);
}

void process_mouse(GLFWwindow* window, double x, double y) {
    if (first_mouse) {
        last_mouse_x = x;
        last_mouse_y = y;
        first_mouse = false;
    }
    float delta_x = last_mouse_x - x;
    float delta_y = last_mouse_y -y;
    last_mouse_x = x;
    last_mouse_y = y;
    yaw += delta_x * MOUSE_SENSITIVITY;
    pitch += delta_y * MOUSE_SENSITIVITY;

    if (pitch > 89.9f) {
        pitch = 89.9f;
    }
    if (pitch < -89.9f) {
        pitch = -89.9f;
    }

    while (yaw > 360.0f) {
        yaw -= 360.0f;
    }
    while (yaw < 0.0f) {
        yaw += 360.0f;
    }

    glm::vec3 front = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch))));
    camera_set_front(front);
}