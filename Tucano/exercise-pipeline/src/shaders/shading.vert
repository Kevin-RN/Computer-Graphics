#version 430

// coming from the mesh properties
in vec4 in_Position;
out vec4 color;
in vec4 in_Color;


// coming from the setUniforms in the shading.hpp file
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;

void main(void)
{	
	vec4 vert = in_Position;
	
	//Translates to the bottom right corner (ex 2 pipelines)
	//vert = vec4(0.5, -0.5, 0, 0) + vert;
	
	//rotates 45 degrees around the z-axis (ex 3 pipelines)
	//vert = rotationMatrix * vert;
	
	// the linearly interpolated color (ex 4 pipelines)
	//color.xyz = in_Position.xyz + vec3(0.5);
	//color.w = 1.0;
	
	//artificial colors using coordinates values (ex 5 pipelines)
	color = in_Color;
	
	// transform position and project
	gl_Position =   projectionMatrix *  viewMatrix * modelMatrix * vert;
}