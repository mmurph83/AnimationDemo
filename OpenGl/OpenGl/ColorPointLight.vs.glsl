// COLOR RENDER - Vertex Shader
#version 400 core

layout (location = 0) in vec4 vVert;
layout (location = 1) in vec2 vTexCoord0;
layout (location = 2) in vec3 vNormal;

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
    gl_Position = proj_matrix * view_matrix * world_matrix * vVert;


	mat4 mvMatrix = view_matrix * world_matrix;
		mat3 mNormalMatrix;
		mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
		mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
		mNormalMatrix[2] = normalize(mvMatrix[2].xyz);
		vec3 vNorm = normalize(mNormalMatrix * vNormal);

		vec4 ecPosition;
		vec3 ecPosition3;
		ecPosition = mvMatrix * vVert;
		ecPosition3 = ecPosition.xyz /ecPosition.w;
		vec3 vLightDir = normalize(vLightPos - ecPosition3);

	float fDot = max(0.0, dot(vNorm, vLightDir)); 
		vs_out.color.rgb = vColor.rgb * fDot;
		vs_out.color.a = vColor.a;
		vs_out.color.xyz = vNormal.xyz;
	// Position is the color + Grey
  //  vs_out.color = vVert * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}