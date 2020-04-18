#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D theTexture;

void main(){
   //color = vec4(1.0f,0.0f,0.0f,1.0f);
   color = texture(theTexture, texCoord);
}