#version 330 core
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec4 v_Color;

out vec4 ourColor;


void main(){
    gl_Position = vec4(v_Pos, 1.0);
    ourColor = v_Color;
}


