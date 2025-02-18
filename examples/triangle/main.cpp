#include "NimbleRenderer.h"
#include <iostream>

namespace nr = NimbleRenderer;

// Points for the Rectangle
float vertices[] = {
	// positions          // colors					// texture coords
	 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
};

unsigned int indices[] = {
	0, 1, 2
};

int main(int argc, char* argv[]) {
	if (!nr::InitWindow(1080, 720, "Triangle")) {
		nr::CloseWindow();
		return -1;
	}
	nr::SetExitKey(0);
	while (!nr::WindowShouldClose()) {
		nr::BeginDrawing();
		nr::ClearBackground(DARKGRAY);
		nr::DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices));
		nr::EndDrawing();
	}
	nr::CloseWindow();
	return 0;
}