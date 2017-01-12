#version 430 core
//the fragment shader colours each fragment (pixel-sized area of the triangle)

//input from the vertex shader
in vec4 vs_color;

//output variable that will be sent to the framebuffer
out vec4 color;

void main(void)
{
	color = vs_color;
}