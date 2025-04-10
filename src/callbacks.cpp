#include "callbacks.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static bool wasResized = false;
static bool keysPressed[GLFW_KEY_LAST + 1] = { false };
static bool keysReleased[GLFW_KEY_LAST + 1] = { false };
// cursor processing
static struct {
	float lastX = 0.0f;
	float lastY = 0.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float sensitivity = 0.1f;
}Cursor;

static struct {
	float yMovement = 0.0f;
}Mouse;

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height){
	glViewport(0, 0, width, height);
	wasResized = true;
}

bool WasResized() {
	return wasResized;
}

void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
	if (key == GLFW_KEY_UNKNOWN) return;
	if (action == GLFW_PRESS) {
		keysPressed[key] = true;
		keysReleased[key] = false;
	}
	else if (action == GLFW_RELEASE) {
		keysPressed[key] = false;
		keysReleased[key] = true;
	}
}

void ResetKeys() {
	for (int x = 0; x < GLFW_KEY_LAST; x++) {
		keysPressed[x] = false;
		keysReleased[x] = false;
	}
}

bool GetKeyPressed(const int key) {
	if (key > GLFW_KEY_LAST) return false;
	return keysPressed[key];
}

bool GetKeyReleased(const int key) {
	if (key > GLFW_KEY_LAST) return false;
	return keysReleased[key];
}

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos) {
	Cursor.xoffset = static_cast<float>(xpos) - Cursor.lastX;
	Cursor.yoffset = static_cast<float>(ypos) - Cursor.lastY;
	Cursor.lastX = static_cast<float>(xpos);
	Cursor.lastY = static_cast<float>(ypos);

	Cursor.xoffset *= Cursor.sensitivity;
	Cursor.yoffset *= Cursor.sensitivity;
}

void SetCursorSensitivity(const float sensitivity) {
	Cursor.sensitivity = sensitivity;
}

glm::vec4 GetCursorInfo() {
	return glm::vec4(Cursor.lastX, Cursor.lastY, Cursor.xoffset, Cursor.yoffset);
}

void ResetMouse() {
	Cursor.xoffset = 0.0f;
	Cursor.yoffset = 0.0f;
	Mouse.yMovement = 0.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Mouse.yMovement = static_cast<float>(yoffset);
}

float GetMouseScrollInfo() {
	return Mouse.yMovement;
}
