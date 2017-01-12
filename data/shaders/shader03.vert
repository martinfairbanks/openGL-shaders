 #version 430 core
//the vertex shader positions each vertex point

//in_position and color are a input vertex attributes
layout (location = 0) in vec4 in_position;	
layout (location = 1) in vec4 color;

//declare VS_OUT as an output interface block
out VS_OUT
{
	vec4 color;		//send color to the next stage
} vs_out;

void main(void)
{
	//gl_Position represents the output position of the vertex
	gl_Position = in_position;	

	//send the color value to the next stage
	vs_out.color = color;
} 