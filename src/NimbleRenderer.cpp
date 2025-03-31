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

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#define NOGDI

#include "NimbleRenderer.h"
#include "utils.h"
// Include GLM for computation
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// Include opengl independent
#include <glad/glad.h>
// include c libraries
#include <chrono>
#include <math.h>
#include <thread>
// include helper functions
#include "callbacks.h"
#include "shader.h"

// stb_image implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Window Management
static GLFWwindow *window;
static int applicationExitKey = GLFW_KEY_ESCAPE;
static unsigned int VBO;
static unsigned int VAO;
static unsigned int EBO;

// Shaders
static Shader mainShader;

// Timing stuff
static auto lastFrameTime = std::chrono::high_resolution_clock::now();
static double frameTime = 0.0f;
static double targetFrameTime = 1000.0f / 60.0f;
static double totalTime = 0.0f;
static unsigned int currentFPS = 0;

// Camera and Perspective settings
static Rectangle BoundingBox;
static glm::mat4 projection = glm::mat4(1.0f);
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
static glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
static glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
static glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
static float rotation = 0.0f;
static float tilt = 0.0f;
static float fov = 75.0f;
static float farPlane = 100.0f;
static float nearPlane = 0.1f;
static float perspectiveX;
static float perspectiveY;
static float perspectiveWidth;
static float perspectiveHeight;

using namespace logging;

namespace NimbleRenderer {
bool InitWindow(int width, int height, const char *name) {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL) {
    logwarning("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  // Setting up callback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    logwarning("Failed to initialize GLAD");
    return false;
  }

  mainShader.setup("", "");
  if (!mainShader.isReady()) {
    logwarning("Failed to create Main Shader");
  }
  glUseProgram(mainShader.ID);
  SetupVertexBuffer();
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Drawing lines only
  // Setting Bounding Box
  BoundingBox = {0, 0, width, height};
  // Setting up image loading
  stbi_set_flip_vertically_on_load(true);
  int major, minor, rev;
  glfwGetVersion(&major, &minor, &rev);
  const char* GL_VERSION_STR = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  loginfo("GLFW Version: %d.%d.%d", major, minor, rev);
  loginfo("OpenGL Version: %s", GL_VERSION_STR);
  const char* VENDOR = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  const char* CARD = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

  loginfo("Graphics Card Vendor: %s", VENDOR);
  loginfo("Graphics Card: %s", CARD);
  int gl_numExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &gl_numExtensions);
  loginfo("Number of supportet OpenGL Extensions: %d", gl_numExtensions);
  /*for (int x = 0; x < gl_numExtensions; x++) {
    std::string str = (const char*)glGetStringi(GL_EXTENSIONS, x);
    log("INFO", "Extension: " + str);
  }*/

  const char* glslLangVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
  loginfo("GLSL Language Version: %s", glslLangVersion);
  // Getting max vertex attributes
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  loginfo("Maximum nr of vertex attributes supported: ", nrAttributes);
  loginfo("Nimble %s loaded successfully\n", VERSION);
  // Setting up BoundingBox
  BoundingBox.x = 0;
  BoundingBox.y = 0;
  glfwGetWindowSize(window, &BoundingBox.width, &BoundingBox.height);
  perspectiveX = 0.0f;
  perspectiveY = 0.0f;
  perspectiveWidth = BoundingBox.width;
  perspectiveHeight = BoundingBox.height;
  return true;
}

void SetupVertexBuffer() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // Position Data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Color Data
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Indices Data
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element
  // buffer object IS stored in the VAO; keep the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void *GetWindowContext() { return window; }

bool WindowShouldClose() {
  // Processing input
  // Closing application with exit key
  if (glfwGetKey(window, applicationExitKey) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  return glfwWindowShouldClose(window);
}

void BeginDrawing() {
  // Calculate timings
  auto currentTime = std::chrono::high_resolution_clock::now();
  double elapsedTime =
      std::chrono::duration<double, std::milli>(currentTime - lastFrameTime)
          .count();
  totalTime = glfwGetTime();
  if (elapsedTime < targetFrameTime) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds((int)(targetFrameTime - elapsedTime)));
    frameTime = targetFrameTime / 1000.0f;
    elapsedTime = frameTime;
  } else {
    frameTime = elapsedTime / 1000.0f;
  }
  currentFPS = static_cast<unsigned int>(1.0f / frameTime);
  lastFrameTime = std::chrono::high_resolution_clock::now();

  // Setting up perspective
  perspectiveX = BoundingBox.x;
  perspectiveY = BoundingBox.y;
  perspectiveWidth = BoundingBox.width;
  perspectiveHeight = BoundingBox.height;

  // Setting up BoundingBox
  if (WasResized()) {
    glfwGetWindowSize(window, &BoundingBox.width, &BoundingBox.height);
    BoundingBox.x = 0;
    BoundingBox.y = 0;
  }
  glUseProgram(mainShader.ID);
}

void EndDrawing() {
  ResetKeys();
  ResetMouse();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void BeginMode3D(const Camera3D &camera) {
  cameraPos = camera.position;
  cameraTarget = camera.target;
  fov = camera.fov;
	// Projection Matrix
	projection = glm::perspective(glm::radians(fov), perspectiveWidth / perspectiveHeight, nearPlane, farPlane);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cameraRight = glm::normalize(glm::cross(camera.up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	view = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);
}

void EndMode3D() {
	view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
	fov = 75.0f;
}

void ClearBackground(Color c) {
  float r = (float)c.r / 255.0f;
  float g = (float)c.g / 255.0f;
  float b = (float)c.b / 255.0f;
  float a = (float)c.a / 255.0f;
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawVertices(float vertices[], size_t vert_size, unsigned int indices[],
                  size_t ind_size, glm::mat4 trans, glm::mat4 view, glm::mat4 model) {
  // Setting up transformation
  mainShader.setMat4("transform", trans);
  mainShader.setMat4("model", model);
  mainShader.setMat4("view", view);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vert_size, vertices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_size, indices, GL_DYNAMIC_DRAW);

  glDrawElements(GL_TRIANGLES, (GLsizei)ind_size, GL_UNSIGNED_INT, 0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element
  // buffer object IS stored in the VAO; keep the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void DrawRectangle(int x, int y, int width, int height, Color c) {
  if (x + width < BoundingBox.x || x > BoundingBox.x + BoundingBox.width ||
      y + height < BoundingBox.y ||
      BoundingBox.y > BoundingBox.y + BoundingBox.height)
    return;
  float r, g, b, a;
  r = c.r / 255.0f;
  g = c.g / 255.0f;
  b = c.b / 255.0f;
  a = c.a / 255.0f;
  float vertices[] = {static_cast<float>(x),
                      static_cast<float>(y),
                      0.0f,
                      r,
                      g,
                      b,
                      a,
                      0.0f,
                      0.0f,
                      static_cast<float>(x + width),
                      static_cast<float>(y),
                      0.0f,
                      r,
                      g,
                      b,
                      a,
                      0.0f,
                      0.0f,
                      static_cast<float>(x),
                      static_cast<float>(y + height),
                      0.0f,
                      r,
                      g,
                      b,
                      a,
                      0.0f,
                      0.0f,
                      static_cast<float>(x + width),
                      static_cast<float>(y + height),
                      0.0f,
                      r,
                      g,
                      b,
                      a,
                      0.0f,
                      0.0f};

  unsigned int indices[6] = {0, 1, 2, 1, 2, 3};
  mainShader.use();
  mainShader.setBool("isTextureSet", 0);
  DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices),
               glm::mat4(1.0f));
}

void DrawTexturePro(Vec2 position, Texture texture, float rotation,
                    Vec3 transformation, Vec3 scale, Color c) {
  float r, g, b, a;
  r = c.r / 255.0f;
  g = c.g / 255.0f;
  b = c.b / 255.0f;
  a = c.a / 255.0f;

  float vertices[] = {
      // positions          // colors    // texture coords
      0.5f,  0.5f,  0.0f, r, g, b, a, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, r, g, b, a, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, r, g, b, a, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, r, g, b, a, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 2, // first triangle
      0, 2, 3  // second triangle
  };
  glDepthMask(GL_FALSE);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.textureID);
  mainShader.use();
  mainShader.setBool("isTextureSet", 1);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(
      trans, glm::radians(rotation),
      glm::vec3(transformation.x, transformation.y, transformation.z));
  trans = glm::scale(trans, glm::vec3(scale.x, scale.y, scale.z));
  DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices), trans);
  glDepthMask(GL_TRUE);
}

void DrawTexturedCube(Vec3 Position, Texture textures[6], float rotation, Vec3 transformation, Vec3 scale, Color c) {
  float diff = glm::distance(cameraPos, glm::vec3(Position.x, Position.y, Position.z));
  if (diff > farPlane)
    return;
  float r, g, b, a;
  r = c.r / 255.0f;
  g = c.g / 255.0f;
  b = c.b / 255.0f;
  a = c.a / 255.0f;
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, r, g, b, a, 0.0f, 0.0f, // 0
       0.5f, -0.5f, -0.5f, r, g, b, a, 1.0f, 0.0f, // 1
       0.5f,  0.5f, -0.5f, r, g, b, a, 1.0f, 1.0f, // 2
      -0.5f,  0.5f, -0.5f, r, g, b, a, 0.0f, 1.0f, // 3

      -0.5f, -0.5f,  0.5f, r, g, b, a, 0.0f, 0.0f, // 4
       0.5f, -0.5f,  0.5f, r, g, b, a, 1.0f, 0.0f, // 5
       0.5f,  0.5f,  0.5f, r, g, b, a, 1.0f, 1.0f, // 6
      -0.5f,  0.5f,  0.5f, r, g, b, a, 0.0f, 1.0f, // 7

      -0.5f,  0.5f,  0.5f, r, g, b, a, 1.0f, 0.0f, // 8
      -0.5f,  0.5f, -0.5f, r, g, b, a, 1.0f, 1.0f, // 9
      -0.5f, -0.5f, -0.5f, r, g, b, a, 0.0f, 1.0f, // 10
      -0.5f, -0.5f,  0.5f, r, g, b, a, 0.0f, 0.0f, // 11

       0.5f,  0.5f,  0.5f, r, g, b, a, 1.0f, 0.0f, // 12
       0.5f,  0.5f, -0.5f, r, g, b, a, 1.0f, 1.0f, // 13
       0.5f, -0.5f, -0.5f, r, g, b, a, 0.0f, 1.0f, // 14
       0.5f, -0.5f,  0.5f, r, g, b, a, 0.0f, 0.0f, // 15

      -0.5f, -0.5f, -0.5f, r, g, b, a, 0.0f, 1.0f, // 16
       0.5f, -0.5f, -0.5f, r, g, b, a, 1.0f, 1.0f, // 17
       0.5f, -0.5f,  0.5f, r, g, b, a, 1.0f, 0.0f, // 18
      -0.5f, -0.5f,  0.5f, r, g, b, a, 0.0f, 0.0f, // 19

      -0.5f,  0.5f, -0.5f, r, g, b, a, 0.0f, 1.0f, // 20
       0.5f,  0.5f, -0.5f, r, g, b, a, 1.0f, 1.0f, // 21
       0.5f,  0.5f,  0.5f, r, g, b, a, 1.0f, 0.0f, // 22
      -0.5f,  0.5f,  0.5f, r, g, b, a, 0.0f, 0.0f, // 23
  };

  unsigned int indices[] = {
    // First Rectangle
    0, 1, 2,
    0, 3, 2,
    // Second Rectangle
    4, 5, 6,
    4, 7, 6,
    // Third Rectangle
    8, 9, 10,
    8, 11, 10,
    // Fourth Rectangle
    12, 13, 14,
    12, 15, 14,
    // Fifth Rectangle
    16, 17, 18,
    16, 19, 18,
    // Sixth Rectangle
    20, 21, 22,
    20, 23, 22
  };

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
  };

	// Model Matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(Position.x, Position.y, Position.z));
	//model = glm::translate(model, cubePositions[x]);
	model = glm::rotate(model, rotation, glm::vec3(transformation.x, transformation.y, transformation.z));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0].textureID);
	mainShader.use();
	mainShader.setBool("isTextureSet", 1);

	DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices), projection, view, model);
}

void DrawTexture3D(Vec3 Position, Texture texture, float rotation, Vec3 transformation, Vec3 scale, Color c) {
  float r, g, b, a;
  r = c.r / 255.0f;
  g = c.g / 255.0f;
  b = c.b / 255.0f;
  a = c.a / 255.0f;

  float vertices[] = {
      // positions          // colors    // texture coords
      0.5f,  0.5f,  0.0f, r, g, b, a, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, r, g, b, a, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, r, g, b, a, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, r, g, b, a, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  // Model Matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(rotation), glm::vec3(transformation.x, transformation.y, transformation.z));
  model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.textureID);
  mainShader.use();
  mainShader.setBool("isTextureSet", 1);

  DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices), projection, view, model);
}

void DrawRectangleRec(Rectangle, Color c) {}

void DrawTexture(int x, int y, const Texture &texture, Color c) {
  float r, g, b, a;
  r = c.r / 255.0f;
  g = c.g / 255.0f;
  b = c.b / 255.0f;
  a = c.a / 255.0f;

  float vertices[] = {
      // positions          // colors    // texture coords
      0.5f,  0.5f,  0.0f, r, g, b, a, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, r, g, b, a, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, r, g, b, a, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, r, g, b, a, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  glDepthMask(GL_FALSE);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.textureID);
  mainShader.use();
  mainShader.setBool("isTextureSet", 1);

  glm::mat4 trans = glm::mat4(1.0f);

  DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices), trans);
  glDepthMask(GL_TRUE);
}

void CloseWindow() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(mainShader.ID);
  glfwDestroyWindow(window);
  glfwTerminate();
  loginfo("Closed Window successfully...");
}

void SetExitKey(int key) { applicationExitKey = key; }

// Timing Functions
double GetFrameTime() { return frameTime; }
double GetTime() { return totalTime; }
unsigned int GetFPS() { return currentFPS; }
void SetFPS(unsigned int fps) { targetFrameTime = 1000.0f / fps; }

// Image Processing
Image LoadImage(std::string filename) {
  Image img;
  img.data =
      stbi_load(filename.c_str(), &img.width, &img.height, &img.nrChannels, 0);
  loginfo("Loaded Image from File %s", filename.c_str());
  return img;
}

void UnloadImage(Image &img) {
  stbi_image_free(img.data);
  img.width = 0;
  img.height = 0;
  img.nrChannels = 0;
  loginfo("Unloaded Image data: %p", img.data);
}

Texture LoadTexture(std::string filename) {
  Image img = LoadImage(filename);
  Texture texture = LoadTexture(img);
  UnloadImage(img);
  return texture;
}

Texture LoadTexture(const Image &img) {
  Texture texture;
  texture.height = img.height;
  texture.width = img.width;
  glBindVertexArray(VAO);
  glGenTextures(1, &texture.textureID);
  glBindTexture(GL_TEXTURE_2D, texture.textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (img.data) {
    if (img.nrChannels > 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, img.data);
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, img.data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    logwarning("Failed to load texture!");
    texture.textureID = 0;
  }
  loginfo("Loaded Texture successfully, Texture ID: %d", texture.textureID);
  glBindVertexArray(0);
  return texture;
}

void UnloadTexture(Texture& texture) {
  glBindVertexArray(VAO);
  glDeleteTextures(1, &texture.textureID);
  loginfo("Unloaded Texture ID: %d", texture.textureID);
  glBindVertexArray(0);
}

// Input Processing
bool IsKeyPressed(const int key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}

bool IsKeyReleased(const int key) {
  return glfwGetKey(window, key) == GLFW_RELEASE;
}

bool IsKeyRepeat(const int key) {
  return glfwGetKey(window, key) == GLFW_REPEAT;
}

bool IsKeyUp(const int key) {
  return !IsKeyPressed(key);
}

bool IsKeyJustPressed(const int key) {
  return GetKeyPressed(key);
}

bool IsKeyJustReleased(const int key) {
  return GetKeyReleased(key);
}

// Mouse settings
void HideCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ShowCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool IsCursorHidden() {
  int mode = glfwGetInputMode(window, GLFW_CURSOR);
  if (mode == GLFW_CURSOR_DISABLED)
    return true;
  return false;
}

Vec2 GetCursorPosition() {
  Vec4 cursorInfo = GetCursorInfo();
  return Vec2(cursorInfo[0], cursorInfo[1]);
}

Vec2 GetCursorOffset() {
  Vec4 cursorInfo = GetCursorInfo();
  return Vec2(cursorInfo[2], cursorInfo[3]);
}

float GetMouseScroll() {
  return GetMouseScrollInfo();
}

// Camera update functions
void UpdateCamera3D(Camera3D& camera, const Vec3 direction, const float speed) {
  
}

} // namespace NimbleRenderer
