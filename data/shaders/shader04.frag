#version 430 core
//the fragment shader colours each fragment (pixel-sized area of the triangle)

//output variable that will be sent to the framebuffer
out vec4 color;

void main(void)
{
	//the built-in variable gl_FragCoord contains the position of the fragment within the window
	color = vec4(	sin(gl_FragCoord.x * 0.55) + 0.5,
					cos(gl_FragCoord.y * 0.25) + 0.5,
					sin(gl_FragCoord.x * 0.25) + 0.8,
					1.0);
	
}