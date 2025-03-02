#include "NimbleRenderer.h"
#include <iostream>

namespace nr = NimbleRenderer;

int main(int argc, char *argv[]) {
  nr::InitWindow(300, 300, "Testing");
  // nr::SetExitKey(0);
  nr::SetFPS(60);
  Texture t = nr::LoadTexture("wall.jpg");
  float rotation = 0.0f;
  float scale = 1.0f;
  while (!nr::WindowShouldClose()) {
    rotation += 0.5f;
    if (rotation > 360.0f)
      rotation = 0.0f;
    scale -= 0.001f;
    if (scale < 0.0f)
      scale = 1.0f;
    nr::BeginDrawing();
    nr::ClearBackground(DARKGRAY);
    nr::DrawRectangle(0, 0, 10, 10, GREEN);
    nr::DrawRectangle(-1, -1, 1, 1, BLUE);
    nr::DrawTexture(0, 0, t, WHITE);
    nr::DrawTexturePro({0.0f, 0.0f}, t, rotation, {0.0f, 0.0f, 1.0f},
                       {scale, scale, scale}, WHITE);
    nr::EndDrawing();
  }
  nr::CloseWindow();
  return 0;
}
