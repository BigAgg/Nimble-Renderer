/*
*	MIT License
*
*	Copyright (c) 2025 BigAgg
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:

*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.

*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/
#ifndef NIMBLERENDERER_H
#define NIMBLERENDERER_H

#include <string>

// Defining structs
namespace shapes {
	typedef struct Rectangle {
		int x;
		int y;
		int width;
		int height;
	} Rectangle;
}

namespace points {
	typedef struct Vec2 {
		float x;
		float y;
	} Vec2;

	typedef struct Vec3 {
		float x;
		float y;
		float z;
	} Vec3;

	typedef struct Vec4 {
		float x;
		float y;
		float z;
		float w;
	} Vec4;
}

typedef struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

typedef struct Image {
	int width, height, nrChannels;
	unsigned char* data;
}Image;

typedef struct Texture {
	int width, height;
	unsigned int textureID;
} Texture;

#define LIGHTGRAY  Color{ 200, 200, 200, 255 }
#define GRAY       Color{ 130, 130, 130, 255 }
#define DARKGRAY   Color{ 80, 80, 80, 255 }
#define YELLOW     Color{ 253, 249, 0, 255 }
#define GOLD       Color{ 255, 203, 0, 255 }
#define ORANGE     Color{ 255, 161, 0, 255 }
#define PINK       Color{ 255, 109, 194, 255 }
#define RED        Color{ 230, 41, 55, 255 }
#define MAROON     Color{ 190, 33, 55, 255 }
#define GREEN      Color{ 0, 228, 48, 255 }
#define LIME       Color{ 0, 158, 47, 255 }
#define DARKGREEN  Color{ 0, 117, 44, 255 }
#define SKYBLUE    Color{ 102, 191, 255, 255 }
#define BLUE       Color{ 0, 121, 241, 255 }
#define DARKBLUE   Color{ 0, 82, 172, 255 }
#define PURPLE     Color{ 200, 122, 255, 255 }
#define VIOLET     Color{ 135, 60, 190, 255 }
#define DARKPURPLE Color{ 112, 31, 126, 255 }
#define BEIGE      Color{ 211, 176, 131, 255 }
#define BROWN      Color{ 127, 106, 79, 255 }
#define DARKBROWN  Color{ 76, 63, 47, 255 }

#define WHITE      Color{ 255, 255, 255, 255 }
#define BLACK      Color{ 0, 0, 0, 255 }
#define BLANK      Color{ 0, 0, 0, 0 }
#define MAGENTA    Color{ 255, 0, 255, 255 }
#define RAYWHITE   Color{ 245, 245, 245, 255 }

namespace NimbleRenderer {
	// Window Handling and Rendering Functions
	bool InitWindow(int width, int height, const char* name);
	bool WindowShouldClose();
	void BeginDrawing();
	void EndDrawing();
	void ClearBackground(Color c);
	void DrawVertices(float vertices[], size_t vert_size, unsigned int indices[], size_t ind_size);
	void CloseWindow();
	void SetupVertexBuffer();
	void* GetWindowContext();
	// Drawing Shapes
	void DrawRectangle(int x, int y, int width, int height, Color c);
	void DrawRectangleRec(shapes::Rectangle rec, Color c);
	void DrawTexture(int x, int y, const Texture& texture, Color c);
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
	void UnloadTexture(Texture texture);
}

#endif
