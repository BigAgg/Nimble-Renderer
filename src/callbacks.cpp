#include "callbacks.h"
#include <GLFW/glfw3.h>

bool wasResized = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	wasResized = true;
}

bool WasResized() {
	return wasResized;
}
