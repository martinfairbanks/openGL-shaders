 #version 430 core
//the vertex shader positions each vertex point

//in_position and color are a input vertex attributes
layout (location = 0) in vec4 in_position;	
layout (location = 1) in vec4 color;

//vs_color is an output that will be sent to the next shader stage
out vec4 vs_color;

void main(void)
{
	//output the value we got in from color
	vs_color = color;

	//gl_Position represents the output position of the vertex
	gl_Position = in_position;	
} 