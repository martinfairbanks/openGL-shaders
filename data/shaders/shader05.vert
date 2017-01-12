 #version 430 core
//the vertex shader positions each vertex point

//in_position and color are a input vertex attributes
layout (location = 0) in vec4 in_position;	

//vs_color is an output that will be sent to the next shader stage
out vec4 vs_color;

void main(void)
{
	//The inputs to the fragment shader are somewhat unlike inputs to other shader
	//stages in that OpenGL interpolates their values across the primitive that’s being rendered. 	
	const vec4 colors[] = vec4[6](	vec4( 1.0, 0.0, 0.0, 1.0),
									vec4( 0.0, 1.0, 0.0, 1.0),
									vec4( 0.0, 0.0, 1.0, 1.0),
									vec4( 1.0, 0.0, 0.0, 1.0),
									vec4( 0.0, 1.0, 0.0, 1.0),
									vec4( 0.0, 0.0, 1.0, 1.0));

	//using gl_VertexID, which is the index of the current vertex
	vs_color = colors[gl_VertexID];

	//gl_Position represents the output position of the vertex
	gl_Position = in_position;	
} 