#version 430 core
//the fragment shader colours each fragment (pixel-sized area of the triangle)

//declare VS_OUT as an output interface block
in VS_OUT
{
	vec4 color;
} fs_in;

//output variable that will be sent to the framebuffer
out vec4 color;

void main(void)
{
	//assign the color from the vertex shader to our output
	color = fs_in.color;
}