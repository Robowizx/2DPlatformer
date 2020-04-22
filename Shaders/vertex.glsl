#version 330

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoord;

uniform mat4 model, projection;
void main(){
    gl_Position = model * vec4(pos,0.0f,1.0f);

    texCoord = tex;
}