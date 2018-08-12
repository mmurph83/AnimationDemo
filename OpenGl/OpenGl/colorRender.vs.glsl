// COLOR RENDER - Vertex Shader
#version 400 core

in vec4 position;

out VS_OUT
{
    vec4 color;
} vs_out;

// Uniforms:
uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec3 vLightPos;
uniform vec4 vColor;

void main(void)
{              
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = proj_matrix * view_matrix * world_matrix * position;

	// Position is the color + Grey
    vs_out.color = vColor;
}