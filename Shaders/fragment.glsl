#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D texunit;
uniform bool debug;

void main(){
 if(debug == 1){
    color = vec4(0.0f,1.0f,0.0f,1.0f);
 }
 else if(debug == 2){
   color = vec4(1.0f,0.0f,0.0f,1.0f);
 }
 else{
   color = texture(texunit,texCoord);
 }  
}