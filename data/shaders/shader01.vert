 #version 430 core
//the vertex shader positions each vertex point
//in_position are a input vertex attribute
layout(location = 0) in vec4 in_position;	

void main(void)
{
	//gl_Position represents the output position of the vertex
	gl_Position = in_position;	
} 