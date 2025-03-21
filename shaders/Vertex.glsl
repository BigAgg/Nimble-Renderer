R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;

void main()
{
    //gl_Position = transform * vec4(aPos, 1.0);
    gl_Position = transform * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
)"
