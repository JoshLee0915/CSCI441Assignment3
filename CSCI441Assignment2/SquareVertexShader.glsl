#version 430 core
out vec4 vertexColor;

void main(void)
{
	// rot matrix
	const mat4 rotY = mat4(
	vec4( 0.7071, 0, 0.7071, 0),
	vec4( 0, 1, 0, 0),
	vec4( -0.7071, 0, 0.7071, 0),
	vec4( 0, 0, 0, 1 ));
	
	const mat4 rotX = mat4(
	vec4( 1, 0, 0, 0),
	vec4( 0, 0.7071, -0.7071, 0),
	vec4( 0, 0.7071, 0.7071, 0),
	vec4( 0, 0, 0, 1 ));
	
	// sides of the cube
	const vec4 vertices[24] = vec4[24](
	vec4(0.5, 0.1, -0.5, 1.0),
	vec4(-0.5, 0.1, -0.5, 1.0),
	vec4(-0.5, 0.1, 0.5, 1.0),
	vec4(0.5, 0.1, 0.5, 1.0),

	vec4(0.5, 0, 0.5, 1.0),
	vec4(-0.5, 0, 0.5, 1.0),
	vec4(-0.5, 0, -0.5, 1.0),
	vec4(0.5, 0, -0.5, 1.0),
 
	vec4(0.5, 0.1, 0.5, 1.0),
	vec4(-0.5, 0.1, 0.5, 1.0),
	vec4(-0.5, 0, 0.5, 1.0),
	vec4(0.5, 0, 0.5, 1.0),
   
	vec4(0.5, 0, -0.5, 1.0),
	vec4(-0.5, 0, -0.5, 1.0),
	vec4(-0.5, 0.1, -0.5, 1.0),
	vec4(0.5, 0.1, -0.5, 1.0),
     
	vec4(-0.5, 0.1, 0.5, 1.0),
	vec4(-0.5, 0.1, -0.5, 1.0),
	vec4(-0.5, 0, -0.5, 1.0),
	vec4(-0.5, 0, 0.5, 1.0),
   
	vec4(0.5, 0.1, -0.5, 1.0),
	vec4(0.5, 0.1, 0.5, 1.0),
	vec4(0.5, 0, 0.5, 1.0),
	vec4(0.5, 0, -0.5, 1.0));
	
	// cube colors
	const vec4 cubeColor[6] = vec4[6](
	vec4(0.0, 1.0, 0.0, 1.0), 	// green
	vec4(1.0, 0.5, 0.0, 1.0), 	// orange
	vec4(1.0, 0.0, 0.0, 1.0),	// red
	vec4(1.0, 1.0, 0.0, 1.0),	// yellow
	vec4(0.0, 0.0, 1.0, 1.0),	// blue
	vec4(1.0, 0.0, 1.0, 1.0));	// magenta
	
	mat4 rot = rotY*rotX;
	
	gl_Position = rot*vertices[gl_VertexID];
	vertexColor = cubeColor[gl_VertexID/4];
}