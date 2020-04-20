#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D theTexture;
uniform bool debug;

void main(){
 if(debug){
    color = vec4(1.0f,0.0f,0.0f,1.0f);
 }
 else{
   color = texture(theTexture,texCoord);
 }  
}