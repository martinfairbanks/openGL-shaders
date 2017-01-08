#version 430 core

//declare color as an output variable
//in fragment shaders all output varibles will be sent to the screen
out vec4 color;

void main(void)
{
	color = vec4(0.0, 1.0, 0.0, 1.0);
}