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

#include "NimbleRenderer.h"
#include "NimbleHelpers.h"
// Include opengl independent
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// include c libraries
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
// include helper functions
#include "callbacks.h"
#include "shader.h"

// stb_image implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Window Management
GLFWwindow* window;
int applicationExitKey = GLFW_KEY_ESCAPE;
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

// Shaders
Shader mainShader;

shapes::Rectangle BoundingBox;

// Timing stuff
auto lastFrameTime = std::chrono::high_resolution_clock::now();
double frameTime = 0.0f;
double targetFrameTime = 1000.0f / 60.0f;
double totalTime = 0.0f;
unsigned int currentFPS = 0;

namespace NimbleRenderer {
	bool InitWindow(int width, int height, const char* name) {
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
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD\n";
      return false;
    }

    mainShader.setup("", "");
    if (!mainShader.isReady()) {
      std::cout << "Failed to create Main Shader\n";
    }
    glUseProgram(mainShader.ID);
    SetupVertexBuffer();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Drawing lines only
    // Setting Bounding Box
    BoundingBox = { 0, 0, width, height };
    // Getting max vertex attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << "\n";
    // Setting up image loading
    stbi_set_flip_vertically_on_load(true);
		return true;
	}

  void SetupVertexBuffer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

  }

	bool WindowShouldClose(){
		// Processing input
		// Closing application with exit key
		if (glfwGetKey(window, applicationExitKey) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		return glfwWindowShouldClose(window);
	}

	void BeginDrawing() {
    // Calculate timings
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration<double, std::milli>(currentTime - lastFrameTime).count();

    if (elapsedTime < targetFrameTime) {
      std::this_thread::sleep_for(std::chrono::milliseconds((int)(targetFrameTime - elapsedTime)));
      frameTime = targetFrameTime / 1000.0f;
    }
    else {
      frameTime = elapsedTime / 1000.0f;
    }
    currentFPS = static_cast<unsigned int>(1.0f / frameTime);
    lastFrameTime = std::chrono::high_resolution_clock::now();
    
    // Setting up BoundingBox
    if (WasResized()) {
      glfwGetWindowSize(window, &BoundingBox.width, &BoundingBox.height);
      BoundingBox.x = 0;
      BoundingBox.y = 0;
    }
    glUseProgram(mainShader.ID);
	}

	void EndDrawing(){
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void ClearBackground(Color c) {
		float r = (float)c.r / 255.0f;
		float g = (float)c.g / 255.0f;
		float b = (float)c.b / 255.0f;
		float a = (float)c.a / 255.0f;
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DrawVertices(float vertices[], size_t vert_size, unsigned int indices[], size_t ind_size) {
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert_size, vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_size, indices, GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, (GLsizei)ind_size, GL_UNSIGNED_INT, 0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
	}

  void DrawRectangle(int x, int y, int width, int height, Color c) {
    if (x + width < BoundingBox.x || x > BoundingBox.x + BoundingBox.width || y + height < BoundingBox.y || BoundingBox.y > BoundingBox.y + BoundingBox.height)
      return;
    float r, g, b, a;
    r = static_cast<float>(c.r) / 255.0f;
    g = static_cast<float>(c.g) / 255.0f;
    b = static_cast<float>(c.b) / 255.0f;
    a = static_cast<float>(c.a) / 255.0f;
    float vertices[] = {
      static_cast<float>(x), static_cast<float>(y), 0.0f, r, g, b, a, 0.0f, 0.0f,
      static_cast<float>(x + width), static_cast<float>(y), 0.0f, r, g, b, a, 0.0f, 0.0f,
      static_cast<float>(x), static_cast<float>(y + height), 0.0f, r, g, b, a, 0.0f, 0.0f,
      static_cast<float>(x + width), static_cast<float>(y + height), 0.0f, r, g, b, a, 0.0f, 0.0f
    };

    unsigned int indices[6] = {
      0, 1, 2,
      1, 2, 3
    };
    glUseProgram(mainShader.ID);
    glUniform1i(glGetUniformLocation(mainShader.ID, "isTextureSet"), 0);
    DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices));
  }

  void DrawRectangleRec(shapes::Rectangle, Color c) {

  }

  void DrawTexture(int x, int y, const Texture& texture, Color c) {
    float r, g, b, a;
    r = c.r / 255.0f;
    g = c.g / 255.0f;
    b = c.b / 255.0f;
    a = c.a / 255.0f;
    float vertices[] = {
      // positions          // colors    // texture coords
       0.5f,  0.5f, 0.0f,   r, g, b, a,  1.0f, 1.0f, // top right
       0.5f, -0.5f, 0.0f,   r, g, b, a,  1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,   r, g, b, a,  0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,   r, g, b, a,  0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
    glUseProgram(mainShader.ID);
    glUniform1i(glGetUniformLocation(mainShader.ID, "isTextureSet"), 1);
    DrawVertices(vertices, sizeof(vertices), indices, sizeof(indices));
  }

	void CloseWindow(){
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(mainShader.ID);
		glfwTerminate();
		std::cout << "Closed Window successfully...\n";
	}

	void SetExitKey(int key){
		applicationExitKey = key;
	}

  // Timing Functions
  double GetFrameTime() {
    return frameTime;
  }
  double GetTime() {
    return totalTime;
  }
  unsigned int GetFPS() {
    return currentFPS;
  }
  void SetFPS(unsigned int fps) {
    targetFrameTime = 1000.0f / fps;
  }

  // Image Processing
  Image LoadImage(std::string filename) {
    Image img;
    img.data = stbi_load(filename.c_str(), &img.width, &img.height, &img.nrChannels, 0);
    return img;
  }

  Texture LoadTexture(std::string filename) {
    Image img = LoadImage(filename);
    Texture texture = LoadTexture(img);
    stbi_image_free(img.data);
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
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    std::cout << "Image has " << img.nrChannels << " Channels\n";
    if(img.data){
      if (img.nrChannels > 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
      }
      else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
      }
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
      std::cout << "Failed to load texture from image!" << "\n";
      texture.textureID = 0;
    }
    return texture;
  }
}
