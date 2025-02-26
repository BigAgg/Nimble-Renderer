#include <iostream>
#include <NimbleRenderer.h>

namespace nr = NimbleRenderer;

int window = 10;

int main(int argc, char* argv[]) {
	nr::InitWindow(300, 300, "Textures");

	Texture t = nr::LoadTexture("wall.jpg");
	Texture t2 = nr::LoadTexture("alpha.png");
	window++;
	unsigned char alpha = 0;
	while (!nr::WindowShouldClose()) {
		nr::BeginDrawing();
		nr::ClearBackground(DARKGRAY);
		nr::DrawTexture(0, 0, t2, WHITE);
		nr::DrawTexture(0, 0, t, {255, 255, 255, alpha});
		nr::EndDrawing();
		alpha++;
		if (alpha == 255)
			alpha = 0;
	}
	nr::CloseWindow();
	return 0;
}