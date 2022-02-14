#version 330 core
out vec4 FragColor;

in vec3 color;
uniform vec4 cellColor;

void main(){
    FragColor = cellColor;
}