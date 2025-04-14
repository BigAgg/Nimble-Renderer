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
  float scale = 0.1f;
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
    float deltatime = static_cast<float>(GetFrameTime());

    // Getting mouse input
    Vec2 mouseOffset = GetCursorOffset();
    yaw += mouseOffset.x;
    pitch -= mouseOffset.y;

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
    
    UpdateCamera3D(&camera, yaw, pitch);

    if (IsKeyPressed(KEY_W)) {
      camera.position += camera.target * 20.0f * deltatime;
    }
    if (IsKeyPressed(KEY_S)) {
      camera.position -= camera.target * 20.0f * deltatime;
    }
    if (IsKeyPressed(KEY_A)) {
      camera.position -= glm::normalize(glm::cross(camera.target, camera.up)) * 20.0f * deltatime;
    }
    if (IsKeyPressed(KEY_D)) {
      camera.position += glm::normalize(glm::cross(camera.target, camera.up)) * 20.0f * deltatime;
    }
    if (IsKeyJustPressed(KEY_M)) {
      ShowCursor();
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode3D(camera);
    for (unsigned int x = 0; x < 14; x++) {
      Vec3 pos = { position.x + x * 1.8f, position.y, position.z };
      DrawTexturedCube(pos, tInfo, (float)GetTime(), {0.5f, 0.0f, 0.0f}, {x*scale, x*scale, x*scale}, c);
    }
    EndMode3D();
    EndDrawing();
  }
  UnloadTexture(t);
  CloseWindow();
  return 0;
}
