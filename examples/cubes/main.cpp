#include "NimbleRenderer.h"
#include <iostream>

using namespace NimbleRenderer;

int main(int argc, char *argv[]) {
  // Program setup
  InitWindow(300, 300, "Cubes");
  SetFPS(60);
  // Preparing Textures
  Texture t = LoadTexture("wall.jpg");
  Texture tInfo[6] = { t, t, t, t, t, t };
  // Preparing Camera
	Camera3D camera;
	camera.target = { 0.0f, 0.0f, -1.0f };
  camera.position = { 0.0f, 0.0f, 3.0f };
  camera.up = { 0.0f, 1.0f, 0.0f };
  camera.fov = 75.0f;
  float speed = 50.0f;
  // Main Loop
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode3D(camera);
    DrawTexturedCube({ 0.0f, 0.0f, 0.0f }, tInfo, (float)GetTime(), { 0.5f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, WHITE);
    EndMode3D();
    EndDrawing();
  }
  // Unloading Textures and closing Window
  UnloadTexture(t);
  CloseWindow();
  return 0;
}
