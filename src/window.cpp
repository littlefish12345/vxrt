#include "extern_includes.hpp"
#include "defines.hpp"
#include "input.hpp"
#include "camera.hpp"
#include "shader.hpp"

GLFWwindow *global_window;

void framebuffer_size_change_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    global_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (global_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(global_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(global_window, framebuffer_size_change_callback);
    glfwSetInputMode(global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(global_window, process_mouse);
    glfwSwapInterval(0);

    camera_init();
    shader_create();
    camera_update();
}

void window_mainloop() {
    double now_time = glfwGetTime();
    double new_time, delta_time;
    double pass_time = 0.0f;
    unsigned int frame_count = 0;
    while(!glfwWindowShouldClose(global_window)) {
        new_time = glfwGetTime();
        delta_time = new_time - now_time;
        now_time = new_time;
        pass_time += delta_time;
        if (pass_time >= 0.2f) {
            glfwSetWindowTitle(global_window, (std::string(WINDOW_TITLE) + " fps:" + std::to_string(frame_count/pass_time)).c_str());
            frame_count = 0;
            pass_time = 0.0f;
        }
        process_keyboard(global_window, delta_time);
        camera_update();

        glClearColor(0.2235f, 0.7725f, 0.7333f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_run();

        glfwSwapBuffers(global_window);
        glfwPollEvents();
        ++frame_count;
    }
}

void window_term() {
    glfwTerminate();
}