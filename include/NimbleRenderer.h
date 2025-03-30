/*
*	MIT License
*
*	Copyright (c) 2025 BigAgg
*
*	Permission is hereby granted, free of charge, to any person obtaining a
copy *	of this software and associated documentation files (the "Software"), to
deal *	in the Software without restriction, including without limitation the
rights *	to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell *	copies of the Software, and to permit persons to whom the
Software is *	furnished to do so, subject to the following conditions:

*	The above copyright notice and this permission notice shall be included
in all *	copies or substantial portions of the Software.

*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE *	SOFTWARE.
*/
#ifndef NIMBLERENDERER_H
#define NIMBLERENDERER_H

#include "glm/glm.hpp"
#include <string>

// Defining structs
typedef struct Rectangle {
  int x;
  int y;
  int width;
  int height;
} Rectangle;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

typedef struct Camera2D {
  Vec2 target;
  float zoom;
  float rotation;
} Camera2D;

typedef struct Camera3D {
  Vec3 position;
  Vec3 target;
  Vec3 up;
  float fov;
  int projection;
} Camera3D;

typedef struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} Color;

typedef struct Image {
  int width, height, nrChannels;
  unsigned char *data;
} Image;

typedef struct Texture {
  int width, height;
  unsigned int textureID;
} Texture;

#define LIGHTGRAY Color{200, 200, 200, 255}
#define GRAY Color{130, 130, 130, 255}
#define DARKGRAY Color{80, 80, 80, 255}
#define YELLOW Color{253, 249, 0, 255}
#define GOLD Color{255, 203, 0, 255}
#define ORANGE Color{255, 161, 0, 255}
#define PINK Color{255, 109, 194, 255}
#define RED Color{230, 41, 55, 255}
#define MAROON Color{190, 33, 55, 255}
#define GREEN Color{0, 228, 48, 255}
#define LIME Color{0, 158, 47, 255}
#define DARKGREEN Color{0, 117, 44, 255}
#define SKYBLUE Color{102, 191, 255, 255}
#define BLUE Color{0, 121, 241, 255}
#define DARKBLUE Color{0, 82, 172, 255}
#define PURPLE Color{200, 122, 255, 255}
#define VIOLET Color{135, 60, 190, 255}
#define DARKPURPLE Color{112, 31, 126, 255}
#define BEIGE Color{211, 176, 131, 255}
#define BROWN Color{127, 106, 79, 255}
#define DARKBROWN Color{76, 63, 47, 255}

#define WHITE Color{255, 255, 255, 255}
#define BLACK Color{0, 0, 0, 255}
#define BLANK Color{0, 0, 0, 0}
#define MAGENTA Color{255, 0, 255, 255}
#define RAYWHITE Color{245, 245, 245, 255}

/* Printable keys */
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348

#define KEY_LAST               KEY_MENU
// Special Keys
#define MOD_SHIFT           0x0001
#define MOD_CONTROL         0x0002
#define MOD_ALT             0x0004
#define MOD_SUPER           0x0008
#define MOD_CAPS_LOCK       0x0010
#define MOD_NUM_LOCK        0x0020
// Mouse Inputs
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3


namespace NimbleRenderer {
// Window Handling and Rendering Functions
bool InitWindow(int width, int height, const char *name);
bool WindowShouldClose();
void BeginDrawing();
void BeginMode3D(const Camera3D &camera);
void EndMode3D();
void EndDrawing();
void ClearBackground(Color c);
void DrawVertices(float vertices[], size_t vert_size, unsigned int indices[],
                  size_t ind_size, glm::mat4 trans, glm::mat4 view = glm::mat4(1.0f), glm::mat4 model = glm::mat4(1.0f));
void CloseWindow();
void SetupVertexBuffer();
void *GetWindowContext();
// Drawing Shapes
void DrawRectangle(int x, int y, int width, int height, Color c);
void DrawRectangleRec(Rectangle rec, Color c);
void DrawTexture(int x, int y, const Texture &texture, Color c);
void DrawTexturePro(Vec2 position, Texture texture, float rotation,
                    Vec3 transformation, Vec3 scale, Color c);
// Draw Objects
void DrawTexture3D(Vec3 Position, Texture texture, float rotation, Vec3 transformation, Vec3 scale, Color c);
void DrawTexturedCube(Vec3 Position, Texture textures[6], float rotation, Vec3 transformation, Vec3 scale, Color c);
// User Input Handling
void SetExitKey(int key);
// Timing functions
double GetFrameTime();
double GetTime();
unsigned int GetFPS();
void SetFPS(unsigned int fps);
// Image Processing
Image LoadImage(std::string filename);
void UnloadImage(Image &img);
Texture LoadTexture(std::string filename);
Texture LoadTexture(const Image &img);
void UnloadTexture(Texture& texture);
// Input processing
bool IsKeyPressed(const int key);
bool IsKeyReleased(const int key);
bool IsKeyRepeat(const int key);
bool IsKeyUp(const int key);
bool IsKeyJustPressed(const int key);
bool IsKeyJustReleased(const int key);
// Mouse Settings
void HideCursor();
void ShowCursor();
bool IsCursorHidden();
Vec2 GetCursorPosition();
Vec2 GetCursorOffset();
// Camera Update functions
void UpdateCamera3D(Camera3D& camera, Vec3 direction, const float speed);
} // namespace NimbleRenderer

#endif
