#include "NimbleRenderer.h"
#include "utils.h"
#include <iostream>

using namespace NimbleRenderer;


int main(int argc, char *argv[]) {
  InitWindow(300, 300, "Testing");
  SetFPS(60);
  HideCursor();
  Texture t = LoadTexture("wall.jpg");
  Texture tInfo[6] = { t, t, t, t, t, t };
  float scale = 1.0f;
  Color c = WHITE;
    
  Vec3 position(0.0f, 0.0f, 0.0f);
  Camera3D camera;
  camera.position = { 0.0f, 0.0f, 3.0f };
  camera.target = { 0.0f, 0.0f, -1.0f };
  camera.up = { 0.0f, 1.0f, 0.0f };
  camera.fov = 75.0f;

  float yaw = -90.0f;
  float pitch = 0.0f;
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_W)) {
      camera.position += camera.target * 20.0f * static_cast<float>(GetFrameTime());
    }
    if (IsKeyPressed(KEY_S)) {
      camera.position -= camera.target * 20.0f * static_cast<float>(GetFrameTime());
    }
    if (IsKeyPressed(KEY_A)) {
      camera.position.x -= 20.0f * static_cast<float>(GetFrameTime());
    }
    if (IsKeyPressed(KEY_D)) {
      camera.position.x += 20.0f * static_cast<float>(GetFrameTime());
    }

    // Getting mouse input
    Vec2 mouseOffset = GetCursorOffset();
    yaw += mouseOffset[0];
    pitch -= mouseOffset[1];

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    Vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.target = glm::normalize(direction);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode3D(camera);
    //position.y += 0.5f;
    for (unsigned int x = 0; x < 14; x++) {
      Vec3 pos = { position.x + x * 1.8f, position.y, position.z };
      DrawTexturedCube(pos, tInfo, (float)GetTime(), {0.5f, 0.0f, 0.0f}, {scale, scale, scale}, c);
    }
    EndMode3D();
    EndDrawing();
  }
  UnloadTexture(t);
  CloseWindow();
  return 0;
}
