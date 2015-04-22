#version 330

in vec3 vertex;
uniform mat4 uberMatrix;

void main(void)
{
	gl_Position=uberMatrix*vec4(vertex,1.0);
}

