#include <GL/gl.h>

#ifndef CNMGR_STRUCTS_H

#define CNMGR_STRUCTS_H

#include "cnmgrDefines.h"

//char cnmgrDefaultVshader[]="#version 330"

struct cnmgrGLShaderProgram
{
	GLuint program;
	int uniformAmount;
	struct cnmgrGLUniform *uniforms;
};

struct cnmgrGLUniform
{
	int cnmgrInternalType;
	int type;
	void *data;
	GLuint uniform;
	GLchar *name;
};

struct cnmgrMeshNode
{
	float position[3];
	float rotation[4][4];
	int vertAmount;
	GLuint vertexBuffer;
	GLuint vertexArray;
	struct cnmgrGLShaderProgram *shaderProgram;
	GLenum drawMode;
	int id;
	int parentType;
	void *parent;
	int nextType;
	void *next;
	int previousType;
	void *previous;
};

struct cnmgrCameraNode
{
	float position[3];
	float rotation[4][4];
	float projection[4][4];
	float fov;
	float zNear;
	float zFar;
	void *parent;
	int nextType;
	void *next;
	int previousType;
	void *previous;
};
#endif