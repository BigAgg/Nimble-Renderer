#include "NimbleRenderer.h"
#include "glm/glm.hpp"
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
  Color c = { 0, 0, 0, 255 };
  c = WHITE;
  char toAdd = 1;
    
  Vec3 position(-12.0f, 10.0f, -5.0f);
  Camera3D camera;
  camera.target = { 0.0f, 0.0f, 3.0f };
  camera.fov = 75.0f;
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  camera.tilt = 0.0f;
  while (!nr::WindowShouldClose()) {
    rotation += 0.5f;
    if (rotation > 360.0f)
      rotation = 0.0f;
    position.y -= 0.1f;
    if (position.y < -10.0f) {
      position.y = 10.0f;
    }
    nr::BeginDrawing();
    nr::ClearBackground(DARKGRAY);
    nr::BeginMode3D(camera);
    //position.y += 0.5f;
    for (unsigned int x = 0; x < 14; x++) {
      Vec3 pos = { position.x + x * 1.8f, position.y, position.z };
      nr::DrawTexturedCube(pos, tInfo, (float)nr::GetTime(), { 0.5f, 0.0f, 0.0f }, {scale, scale, scale }, c);
    }
    nr::EndMode3D();
    nr::EndDrawing();
  }
  nr::UnloadTexture(t);
  nr::CloseWindow();
  return 0;
}
