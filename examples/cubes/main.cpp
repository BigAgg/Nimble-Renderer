#include "NimbleRenderer.h"
#include <iostream>

namespace nr = NimbleRenderer;

int main(int argc, char *argv[]) {
  nr::InitWindow(300, 300, "Testing");
  // nr::SetExitKey(0);
  nr::SetFPS(60);
  Texture t = nr::LoadTexture("wall.jpg");
  Texture tInfo[6] = { t, t, t, t, t, t };
  //nr::UnloadTexture(t);
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
    //nr::DrawRectangle(0, 0, 10, 10, GREEN);
    //nr::DrawRectangle(-1, -1, 1, 1, BLUE);
    //nr::DrawTexture3D({ 0.0f, 0.5f, 3.0f }, t, -55.0f, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, RED);
    nr::DrawTexturedCube({ 0.0f, 0.0f, 0.0f }, tInfo, (float)nr::GetTime(), { 0.5f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, WHITE);
    nr::EndDrawing();
  }
  nr::UnloadTexture(t);
  nr::CloseWindow();
  return 0;
}
