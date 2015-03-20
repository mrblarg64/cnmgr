#include "cnmgrStructs.h"
void printShtuuuf();
int cnmgrCreateFragShaderFromFile(GLuint *fshader,char *shaderLocation);
int cnmgrCreateVertexShaderFromFile(GLuint *vshader,char *shaderLocation);
void cnmgrCreateGLProgramFromFragmentShader(struct cnmgrGLShaderProgram *program,GLuint *fshader,int uniformAmount);
void cnmgrCreateGLProgramCustom(struct cnmgrGLShaderProgram *program,GLuint *fshader,GLuint *vshader,GLuint *gshader,int uniformAmount);
void cnmgrSetupGLUniform(struct cnmgrGLShaderProgram *program,int uniformNumber,char *name,int type);
void cnmgrRenderScene(struct cnmgrCameraNode **camera,int width,int height);
void cnmgrAddCameraSceneNode(struct cnmgrCameraNode **camera,GLdouble fov,GLdouble zNear,GLdouble zFar);
struct cnmgrMeshNode *cnmgrAddMeshNodeFromNonIndexedFloatArray(struct cnmgrCameraNode **camera,int id,void *parent,int parentType,int floatsPerVertex,int vertAmount,float *verts);