#include <GLFW/glfw3.h>
#include "cnmgr.h"
#include <stdio.h>

GLuint tempBuf;
GLuint vshader;
GLuint fshader;
GLFWwindow *window;
struct cnmgrCameraNode *camera;
struct cnmgrGLShaderProgram tempProgram;

//temp make test

openWindow()
{
	glfwInit();
	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES,16);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	window=glfwCreateWindow(640,480,"hello glfw-3",NULL,NULL);
	glfwMakeContextCurrent(window);
	//glfwEnable(GLFW_STICKY_KEYS);
	//vsync...
	glfwSwapInterval(1);




	GLfloat tempTriangle[]={-1,0,0,0,1,0,1,0,0};
	
	cnmgrCreateFragShaderFromFile(&fshader,"fshader");
	cnmgrCreateVertexShaderFromFile(&vshader,"vshader");
	cnmgrCreateGLProgramCustom(&tempProgram,&fshader,&vshader,NULL,1);
	cnmgrSetupGLUniform(&tempProgram,0,"uberMatrix",CNMGR_GL_UNIFORM_MATRIX_4FV);

	cnmgrAddCameraSceneNode(&camera,65,1,100);
	cnmgrAddMeshNodeFromNonIndexedFloatArray(&camera,0,NULL,0,3,9,tempTriangle);

	printShtuuuf();
}

loopThing()
{
	int width;
	int height;
	float z=20;

	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window,&width,&height);
		glViewport(0,0,width,height);
		glClearColor(0,1,0,0);
		glClear(GL_COLOR_BUFFER_BIT);

//uncomment next line
		cnmgrRenderScene(&camera,width,height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

cleanUp()
{
	glfwTerminate();
}

main()
{
	openWindow();
	loopThing();
	cleanUp();
}