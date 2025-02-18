R"(
#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform bool isTextureSet;

void main()
{
    if(ourColor.a < 0.01) discard;
    if(isTextureSet){
      FragColor = texture(ourTexture, TexCoord) * ourColor;
    }
    else{
      FragColor = ourColor;
    }
}
)"
