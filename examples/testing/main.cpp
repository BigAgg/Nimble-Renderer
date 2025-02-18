#include "NimbleRenderer.h"
#include <iostream>

namespace nr = NimbleRenderer;

int main(int argc, char* argv[]) {
	nr::InitWindow(300, 300, "Testing");
	//nr::SetExitKey(0);
	nr::SetFPS(60);
	while (!nr::WindowShouldClose()) {
		nr::BeginDrawing();
		nr::ClearBackground(DARKGRAY);
		nr::DrawRectangle(0, 0, 10, 10, GREEN);
		nr::DrawRectangle(-1, -1, 1, 1, BLUE);
		nr::EndDrawing();
	}
	nr::CloseWindow();
	return 0;
}