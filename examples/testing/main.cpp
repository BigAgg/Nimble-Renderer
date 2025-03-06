#include "NimbleRenderer.h"
#include "glm/glm.hpp"
#include <iostream>

namespace nr = NimbleRenderer;

// Points for the Rectangle
float vertices[] = {
    // positions        // colors					      //texture coords
    -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top left
    1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f    // top right
};

unsigned int indices[] = {0, 3, 1, 0, 2, 1};

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
  char toAdd = 1;
    
  Vec3 view(0.0f, 0.0f, 3.0f);
  while (!nr::WindowShouldClose()) {
    if (toAdd == 1) {
      c.r += 2;
      c.g += 2;
      c.b += 2;
      c.a += 2;
    }
    else {
      c.r -= 2;
      c.b -= 2;
      c.g -= 2;
      c.a -= 2;
    }
    if (c.a <= 4)
      toAdd = 1;
    if (c.a >= 253)
      toAdd = -1;
    //c.a += toAdd*4;
    rotation += 0.5f;
    if (rotation > 360.0f)
      rotation = 0.0f;
    scale -= 0.001f;
    if (scale < 0.0f)
      scale = 1.0f;
    nr::BeginDrawing();
    nr::ClearBackground(DARKGRAY);
    //nr::DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices), glm::mat4{ 1.0f });
    //nr::DrawRectangle(0, 0, 10, 10, GREEN);
    //nr::DrawRectangle(-1, -1, 1, 1, BLUE);
    //nr::DrawTexture3D({ 0.0f, 0.5f, 3.0f }, t, -55.0f, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, RED);
    nr::DrawTexturedCube(view, tInfo, (float)nr::GetTime(), { -(float)nr::GetTime(), (float)nr::GetTime(), (float)nr::GetTime()}, {scale, scale, scale}, c);
    nr::EndDrawing();
  }
  nr::UnloadTexture(t);
  nr::CloseWindow();
  return 0;
}
