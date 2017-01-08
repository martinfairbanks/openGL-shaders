 #version 430 core

layout(location = 0) in vec4 in_position;	
void main(void)
{
	//gl_Position represents the output position of the vertex
	gl_Position = in_position;		
} 