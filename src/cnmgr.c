#include "cnmgrStructs.h"
#include "cnmgrConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

GLuint cnmgrDefaultVertShader;
GLuint cnmgrDefaultFragShader;
struct cnmgrGLShaderProgram cnmgrDefaultShaderProgram;

int cnmgrInit()
{
	printf("cnmgr starting up...\n");
	printf("compiling default shaders...");
	cnmgrCreateVertexShaderFromBuffer(&cnmgrDefaultVertShader,cnmgrBuiltInVertexShader);
	cnmgrCreateFragShaderFromBuffer(&cnmgrDefaultFragShader,cnmgrBuiltInFragShader);
	printf("done!\n");
	printf("creating default program...");
	cnmgrCreateGLProgramFromFragmentShader(&cnmgrDefaultShaderProgram,&cnmgrDefaultFragShader,0);
	printf("done!\n");
	printf("startup complete!\n");
}

int cnmgrCreateFragShaderFromBuffer(GLuint *fshader,char *buffer)
{
	GLint logLength=0;
	GLchar *compilerLog;
	GLsizei unknownVariable=0;

	*fshader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fshader,1,&buffer,NULL);
	glCompileShader(*fshader);
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
int cnmgrCreateFragShaderFromFile(GLuint *fshader,char *shaderLocation)
{
	char *shaderSource;
	FILE *fp=NULL;
	unsigned int size;
	int retval;

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
	retval=cnmgrCreateFragShaderFromBuffer(fshader,shaderSource);
	free(shaderSource);
	return retval;
}

int cnmgrCreateVertexShaderFromBuffer(GLuint *vshader,char *buffer)
{
	GLint logLength=0;
	GLchar *compilerLog;
	GLsizei unknownVariable=0;

	*vshader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vshader,1,&buffer,NULL);
	glCompileShader(*vshader);
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
}

int cnmgrCreateVertexShaderFromFile(GLuint *vshader,char *shaderLocation)
{
	char *shaderSource;
	FILE *fp=NULL;
	unsigned int size;
	int retval;

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
	retval=cnmgrCreateVertexShaderFromBuffer(vshader,shaderSource);
	free(shaderSource);
	return retval;
}

void cnmgrSetupGLUniform(struct cnmgrGLShaderProgram *program,int uniformNumber,char *name,int type,int internalType)
{
	//Greater than or equal too because off by one for arrays
	if (uniformNumber >= program->uniformAmount)
	{
		printf("CNMGR - cnmgrSetupGLUniform - Programmer attempted to modify nonexistant GL uniform, aborting...\n");
		return;
	}
	((*program).uniforms[uniformNumber]).name=malloc(strlen(name)+1);
	memcpy(((*program).uniforms[uniformNumber]).name,name,strlen(name)+1);
	((*program).uniforms[uniformNumber]).uniform=glGetUniformLocation(program->program,name);
	((*program).uniforms[uniformNumber]).type=type;
	((*program).uniforms[uniformNumber]).cnmgrInternalType=internalType;
}

void cnmgrSetupGLUniformCustom(struct cnmgrGLShaderProgram *program,int uniformNumber,char *name,int type,void *dataPointer)
{
	//Greater than or equal too because off by one for arrays
	if (uniformNumber >= program->uniformAmount)
	{
		printf("CNMGR - cnmgrSetupGLUniformCustom - Programmer attempted to modify nonexistant GL uniform, aborting...\n");
		return;
	}
	((*program).uniforms[uniformNumber]).name=malloc(strlen(name)+1);
	memcpy(((*program).uniforms[uniformNumber]).name,name,strlen(name)+1);
	((*program).uniforms[uniformNumber]).uniform=glGetUniformLocation(program->program,name);
	((*program).uniforms[uniformNumber]).type=type;
	((*program).uniforms[uniformNumber]).data=dataPointer;
	((*program).uniforms[uniformNumber]).cnmgrInternalType=1;
}

void cnmgrCreateGLProgramFromFragmentShader(struct cnmgrGLShaderProgram *program,GLuint *fshader,int uniformAmount)
{
	program->program=glCreateProgram();
	glAttachShader(program->program,*fshader);
	glAttachShader(program->program,cnmgrDefaultVertShader);
	glLinkProgram(program->program);
	program->uniforms=malloc(sizeof(struct cnmgrGLUniform)*(1+uniformAmount));
	program->uniformAmount=uniformAmount+1;
	cnmgrSetupGLUniform(program,0,"uberMatrix",CNMGR_GL_UNIFORM_MATRIX_4FV,CNMGR_INTERNAL_TYPE_UBERMATRIX);
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

void cnmgrRenderScene(struct cnmgrCameraNode **camera,int width,int height)
{
	void *current;
	int currentType;
	int notLoopedBackToCamera=1;
	float tanFOV;
	cnmgrMatrix4x4 uberMatrix;
	int currentUniform;

//setup projection matrix
	tanFOV=tanf((((*camera)->fov)/360.0f)*(M_PI*2.0f));
	memset(&(*camera)->projection,0,sizeof(cnmgrMatrix4x4));
	(*camera)->projection[0][0]=1.0f/tanFOV;
	(*camera)->projection[1][1]=((float)width/(float)height)/tanFOV;
	(*camera)->projection[2][2]=((*camera)->zFar+(*camera)->zNear)/((*camera)->zFar-(*camera)->zNear);
	(*camera)->projection[2][3]=1.0f;
	(*camera)->projection[3][2]=(-2.0f*(*camera)->zFar*(*camera)->zNear)/((*camera)->zFar-(*camera)->zNear);
	//projection={{1.0f/tanFOV,0.0f,0.0f,0.0f},{0.0f,((float)height/(float)width)/tanFOV,0.0f,0.0f},{0.0f,0.0f,(zFar+zNear)/(zFar-zNear),1.0f},{0.0f,0.0f,-2.0f*zFar*zNear/(zFar-zNear),0.0f}};
	//(*camera)->projection={{1.0f/tanFOV,0.0f,0.0f,0.0f},{0.0f,((float)height/(float)width)/tanFOV,0.0f,0.0f},{0.0f,0.0f,(zFar+zNear)/(zFar-zNear),1.0f},{0.0f,0.0f,-2.0f*zFar*zNear/(zFar-zNear),0.0f}};
	//memcpy(&(*camera)->projection,

//check if there is anything to draw
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
				cnmgrMeshNodeGenerateUberMatrix((struct cnmgrMeshNode*)current,camera,&uberMatrix);
				glUseProgram(((struct cnmgrMeshNode*)current)->shaderProgram->program);
				currentUniform=0;
				while (currentUniform!=((struct cnmgrMeshNode*)current)->shaderProgram->uniformAmount)
				{
					switch (((struct cnmgrMeshNode*)current)->shaderProgram->uniforms[currentUniform].type)
					{
						case CNMGR_GL_UNIFORM_1F:
							break;
						case CNMGR_GL_UNIFORM_2F:
							break;
						case CNMGR_GL_UNIFORM_3F:
							break;
						case CNMGR_GL_UNIFORM_4F:
							break;
						case CNMGR_GL_UNIFORM_1I:
							break;
						case CNMGR_GL_UNIFORM_2I:
							break;
						case CNMGR_GL_UNIFORM_3I:
							break;
						case CNMGR_GL_UNIFORM_4I:
							break;
						case CNMGR_GL_UNIFORM_1UI:
							break;
						case CNMGR_GL_UNIFORM_2UI:
							break;
						case CNMGR_GL_UNIFORM_3UI:
							break;
						case CNMGR_GL_UNIFORM_4UI:
							break;
						case CNMGR_GL_UNIFORM_1FV:
							break;
						case CNMGR_GL_UNIFORM_2FV:
							break;
						case CNMGR_GL_UNIFORM_3FV:
							break;
						case CNMGR_GL_UNIFORM_4FV:
							break;
						case CNMGR_GL_UNIFORM_1IV:
							break;
						case CNMGR_GL_UNIFORM_2IV:
							break;
						case CNMGR_GL_UNIFORM_3IV:
							break;
						case CNMGR_GL_UNIFORM_4IV:
							break;
						case CNMGR_GL_UNIFORM_1UIV:
							break;
						case CNMGR_GL_UNIFORM_2UIV:
							break;
						case CNMGR_GL_UNIFORM_3UIV:
							break;
						case CNMGR_GL_UNIFORM_4UIV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_2FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_3FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_4FV:
							if (CNMGR_INTERNAL_TYPE_UBERMATRIX==((struct cnmgrMeshNode*)current)->shaderProgram->uniforms[currentUniform].cnmgrInternalType)
							{
								glUniformMatrix4fv(((struct cnmgrMeshNode*)current)->shaderProgram->uniforms[currentUniform].uniform,1,GL_FALSE,uberMatrix);
							}
							break;
						case CNMGR_GL_UNIFORM_MATRIX_2X3FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_3X2FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_2X4FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_4X2FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_3X4FV:
							break;
						case CNMGR_GL_UNIFORM_MATRIX_4X3FV:
							break;
					}
					currentUniform++;
				}
				glBindVertexArray(((struct cnmgrMeshNode*)current)->vertexArray);

				glDrawArrays(((struct cnmgrMeshNode*)current)->drawMode,0,((struct cnmgrMeshNode*)current)->vertAmount);

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

void cnmgrAddCameraSceneNode(struct cnmgrCameraNode **camera,float fov,float zNear,float zFar)
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
	mesh->vertAmount=vertAmount;
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

cnmgrMatrixMultiply4x4(float matrixOne[4][4],float matrixTwo[4][4],float outputMatrix[4][4])
{
	int notDone=1;
	int x=0;
	int y=0;

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

cnmgrMeshNodeGenerateUberMatrix(struct cnmgrMeshNode *mesh,struct cnmgrCameraNode **camera,cnmgrMatrix4x4 *uberMatrix)
{
	cnmgrMatrix4x4 translation;
	cnmgrMatrix4x4 current;

	//not the fastest way of generating the translation/identity matrix
	translation[0][0]=1.0f;
	translation[1][1]=1.0f;
	translation[2][2]=1.0f;
	translation[3][3]=1.0f;
	//x
	translation[3][0]=mesh->position[0];
	//y
	translation[3][1]=mesh->position[1];
	//z
	translation[3][2]=mesh->position[2];

	cnmgrMatrixMultiply4x4(translation,(*camera)->projection,*uberMatrix);

	//memcpy(uberMatrix,&current,sizeof(cnmgrMatrix4x4));
}

cnmgrMeshNodeSetDefaultValues(struct cnmgrMeshNode *mesh)
{
	memset(&mesh->position,0,sizeof(mesh->position));
	memset(&mesh->rotation,0,sizeof(mesh->rotation));
	mesh->drawMode=GL_TRIANGLES;
	mesh->shaderProgram=&cnmgrDefaultShaderProgram;
}