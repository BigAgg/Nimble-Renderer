#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
bool WasResized();

void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);
void ResetKeys();
bool GetKeyPressed(const int key);
bool GetKeyReleased(const int key);

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos);
void SetCursorSensitivity(const float sensitivity);
glm::vec4 GetCursorInfo();
void ResetMouse();

#endif
