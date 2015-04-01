#include "cnmgrStructs.h"
#include "cnmgrConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int defaultProgramCreated=0;
int defaultVertexShaderCreated=0;

int cnmgrCreateFragShaderFromFile(GLuint *fshader,char *shaderLocation)
{
	char *shaderSource;
	FILE *fp=NULL;
	unsigned int size;
	GLint logLength=0;
	GLchar *compilerLog;
	GLsizei unknownVariable=0;

	fp=fopen(shaderLocation,"r");
	if (fp==NULL)
	{
		printf("CNMGR - cnmgrCreateFragShaderFromFile - unable to open specified file\n");
		return 1;
	}
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	fseek(fp,0,SEEK_SET);
	shaderSource=malloc(size+1);
	fread(shaderSource,size,1,fp);
	shaderSource[size]='\0';
	fclose(fp);
	*fshader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fshader,1,&shaderSource,NULL);
	glCompileShader(*fshader);
	free(shaderSource);
	glGetShaderiv(*fshader,GL_INFO_LOG_LENGTH,&logLength);
	if (logLength>1)
	{
		printf("CNMGR - cnmgrCreateFragShaderFromFile - Shader Compile failure, Dumping log...\n");
		compilerLog=malloc(logLength);
		glGetInfoLogARB(*fshader,logLength,&unknownVariable,compilerLog);
		printf("%s\n",compilerLog);
		free(compilerLog);
		printf("CNMGR - cnmgrCreateFragShaderFromFile - Log Dump Complete\n");
		return 1;
	}
	return 0;
}

void printShtuuuf()
{
	printf("%s\n",cnmgrDefaultVertexShader);
}

int cnmgrCreateVertexShaderFromFile(GLuint *vshader,char *shaderLocation)
{
	char *shaderSource;
	FILE *fp=NULL;
	unsigned int size;
	GLint logLength=0;
	GLchar *compilerLog;
	GLsizei unknownVariable=0;

	fp=fopen(shaderLocation,"r");
	if (fp==NULL)
	{
		printf("CNMGR - cnmgrCreateVertexShaderFromFile - unable to open specified file\n");
		return 1;
	}
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	fseek(fp,0,SEEK_SET);
	shaderSource=malloc(size+1);
	fread(shaderSource,size,1,fp);
	shaderSource[size]='\0';
	fclose(fp);
	*vshader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vshader,1,&shaderSource,NULL);
	glCompileShader(*vshader);
	free(shaderSource);
	glGetShaderiv(*vshader,GL_INFO_LOG_LENGTH,&logLength);
	if (logLength>1)
	{
		printf("CNMGR - cnmgrCreateVertexShaderFromFile - Shader Compile failure, Dumping log...\n");
		compilerLog=malloc(logLength);
		glGetInfoLogARB(*vshader,logLength,&unknownVariable,compilerLog);
		printf("%s\n",compilerLog);
		free(compilerLog);
		printf("CNMGR - cnmgrCreateVertexShaderFromFile - Log Dump Complete\n");
		return 1;
	}
	return 0;
}

void cnmgrCreateGLProgramFromFragmentShader(struct cnmgrGLShaderProgram *program,GLuint *fshader,int uniformAmount)
{
	//program->program=glCreateProgram();
	//glAttachShader(program->program,*fshader);
	//glAttachShader(program->program,*vshader);
	//glLinkProgram(program->program);
	//program->uniforms=malloc(sizeof(struct cnmgrGLUniform)*(1+uniformAmount));
}

void cnmgrCreateGLProgramCustom(struct cnmgrGLShaderProgram *program,GLuint *fshader,GLuint *vshader,GLuint *gshader,int uniformAmount)
{
	program->program=glCreateProgram();
	glAttachShader(program->program,*fshader);
	glAttachShader(program->program,*vshader);
	if (gshader!=NULL)
	{
		printf("CNMGR - cnmgrCreateGLProgram - Geometry Shader Support Not yet implemented\n");
	}
	glLinkProgram(program->program);
	//printf("%s\n",cnmgrDefaultFshader);
	program->uniformAmount=uniformAmount;
	if (uniformAmount!=0)
	{
		program->uniforms=malloc(sizeof(struct cnmgrGLUniform)*uniformAmount);
	}
}

void cnmgrSetupGLUniform(struct cnmgrGLShaderProgram *program,int uniformNumber,char *name,int type)
{
	//Greater than or equal too because off by one for arrays
	if (uniformNumber >= program->uniformAmount)
	{
		printf("CNMGR - cnmgrSetupUniform - Programmer attempted to modify nonexistant GL uniform, aborting...\n");
		return;
	}
	((*program).uniforms[uniformNumber]).name=malloc(strlen(name)+1);
	memcpy(((*program).uniforms[uniformNumber]).name,name,strlen(name)+1);
	((*program).uniforms[uniformNumber]).uniform=glGetUniformLocation(program->program,name);
	((*program).uniforms[uniformNumber]).type=type;
	//never go full retard...
	//dont mess with this, im taking a pointer to the data in the functions input, there is no need to malloc space, that should be handled by whoever calls this function
	//((*program).uniforms[uniformNumber]).data=data;
}

void cnmgrRenderScene(struct cnmgrCameraNode **camera,int width,int height)
{
	void *current;
	int currentType;
	int notLoopedBackToCamera=1;
	float tanFOV;

	tanFOV=tan((((*camera)->fov)/360.0f)*(M_PI*2.0f));
	//(*camera)->projection={{1.0f/tanFOV,0.0f,0.0f,0.0f},{0.0f,((float)height/(float)width)/tanFOV,0.0f,0.0f},{0.0f,0.0f,(zFar+zNear)/(zFar-zNear),1.0f},{0.0f,0.0f,-2.0f*zFar*zNear/(zFar-zNear),0.0f}};
	//memcpy(&(*camera)->projection,

	if ((*camera)->next==NULL)
	{
		return;
	}
	currentType=(*camera)->nextType;
	current=(*camera)->next;
	while (notLoopedBackToCamera)
	{
		switch (currentType)
		{
			case CNMGR_NODE_TYPE_MESH:
				cnmgrMeshNodeGenerateUberMatrix((struct cnmgrMeshNode*)current,camera);
				glUseProgram(((struct cnmgrMeshNode*)current)->shaderProgram->program);
				glBindVertexArray(((struct cnmgrMeshNode*)current)->vertexArray);

				//glDrawArrays(((struct cnmgrMeshNode*)current)->drawMode,0,

				currentType=((struct cnmgrMeshNode*)current)->nextType;
				current=((struct cnmgrMeshNode*)current)->next;
				if (current==(*camera))
				{
					notLoopedBackToCamera=0;
				}
				break;
		}
	}
}

void cnmgrAddCameraSceneNode(struct cnmgrCameraNode **camera,GLdouble fov,GLdouble zNear,GLdouble zFar)
{
	(*camera)=malloc(sizeof(struct cnmgrCameraNode));
	memset(*camera,0,sizeof(struct cnmgrCameraNode));
	(*camera)->fov=fov;
	(*camera)->zNear=zNear;
	(*camera)->zFar=zFar;
}

struct cnmgrMeshNode *cnmgrAddMeshNodeFromNonIndexedFloatArray(struct cnmgrCameraNode **camera,int id,void *parent,int parentType,int floatsPerVertex,int vertAmount,float *verts)
{
	struct cnmgrMeshNode *mesh;

	mesh=malloc(sizeof(struct cnmgrMeshNode));
	cnmgrMeshNodeSetupPointers(mesh,camera,parent,parentType,id);
	cnmgrBufferVertexArrayMesh(CNMGR_NON_INDEXED_FLOAT_ARRAY,mesh,floatsPerVertex,vertAmount,verts);
	cnmgrMeshNodeSetDefaultValues(mesh);
	return mesh;
}

cnmgrMeshNodeSetupPointers(struct cnmgrMeshNode *mesh,struct cnmgrCameraNode **camera,void *parent,int parentType,int id)
{
	if ((*camera)->previous==NULL)
	{
		(*camera)->previousType=CNMGR_NODE_TYPE_MESH;
		(*camera)->previous=mesh;
		(*camera)->nextType=(*camera)->previousType;
		(*camera)->next=(*camera)->previous;
		mesh->next=*camera;
		mesh->previous=*camera;
		mesh->nextType=CNMGR_NODE_TYPE_CAMERA;
		mesh->previousType=CNMGR_NODE_TYPE_CAMERA;
	}
	else
	{
		if ((*camera)->previousType==CNMGR_NODE_TYPE_MESH)
		{
			mesh->previous=(*camera)->previous;
			mesh->previousType=(*camera)->previousType;
			mesh->next=*camera;
			mesh->nextType=CNMGR_NODE_TYPE_CAMERA;
			((struct cnmgrMeshNode*)((*camera)->previous))->next=mesh;
			((struct cnmgrMeshNode*)((*camera)->previous))->nextType=CNMGR_NODE_TYPE_MESH;
		}
	}
	if (parent!=NULL)
	{
		printf("CNMGR - cnmgrSetupMeshNodePointers - parenting not yet implemented, ignoring parent\n");
	}
	mesh->id=id;
}

cnmgrBufferVertexArrayMesh(int type,struct cnmgrMeshNode *mesh,int floatsPerVertex,int vertAmount,void *data)
{
	switch (type)
	{
		case CNMGR_NON_INDEXED_FLOAT_ARRAY:
			glGenVertexArrays(1,&(mesh->vertexArray));
			glBindVertexArray(mesh->vertexArray);
			glGenBuffers(1,&(mesh->vertexBuffer));
			glBindBuffer(GL_ARRAY_BUFFER,mesh->vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertAmount,data,GL_STATIC_DRAW);
			if (floatsPerVertex<1||floatsPerVertex>4)
			{
				printf("CNMGR - cnmgrBufferVertexArrayMesh - floatsPerVertex (for use with size in glVertexAttribPointer) must be between 1, 2, 3, or 4 programmer called %d continuing, expect problems\n",floatsPerVertex);
			}
			glVertexAttribPointer(0,floatsPerVertex,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
			break;
	}
	return;
}

cnmgrMatrixMultiply4x4(float **matrixOne,float **matrixTwo,float **outputMatrix)
{
	int notDone=1;
	int x=0;
	int y=0;

	//printf("CNMGR - cnmgrMatrixMultiply4x4 - This function doesnt do anything right now\n");
	while (notDone)
	{
		outputMatrix[y][x] = matrixOne[y][0]*matrixTwo[0][x] + matrixOne[y][1]*matrixTwo[1][x] + matrixOne[y][2]*matrixTwo[2][x] + matrixOne[y][3]*matrixTwo[3][x];
		x++;
		if (x==4)
		{
			x=0;
			y++;
			if (y==4)
			{
				notDone=0;
			}
		}
	}
}

cnmgrMeshNodeGenerateUberMatrix(struct cnmgrMeshNode *mesh,struct cnmgrCameraNode **camera)
{
	printf("CNMGR - cnmgrMeshNodeGenerateUberMatrix - This function doesnt do anything right now\n");

	
}

cnmgrMeshNodeSetDefaultValues(struct cnmgrMeshNode *mesh)
{
	memset(&mesh->position,0,sizeof(mesh->position));
	memset(&mesh->rotation,0,sizeof(mesh->rotation));
	mesh->drawMode=GL_TRIANGLES;
}