#version 430 core
//the fragment shader colours each fragment (pixel-sized area of the triangle)
//output variable that will be sent to the framebuffer
out vec4 color;

void main(void)
{
	//assign a fixed color
	color = vec4(0.0, 0.0, 1.0, 1.0);
}