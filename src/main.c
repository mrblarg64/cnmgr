#include <GLFW/glfw3.h>
#include "cnmgr.h"
#include <stdio.h>

GLuint tempBuf;
GLuint vshader;
GLuint fshader;
GLFWwindow *window;
struct cnmgrCameraNode *camera;
struct cnmgrGLShaderProgram tempProgram;
struct cnmgrMeshNode *myTriangle;

openWindow()
{
	glfwInit();
	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES,16);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	window=glfwCreateWindow(640,480,"hello glfw-3",NULL,NULL);
	//window=glfwCreateWindow(1366,768,"hello glfw-3",NULL,NULL);
	glfwMakeContextCurrent(window);
	//glfwEnable(GLFW_STICKY_KEYS);
	//vsync...
	glfwSwapInterval(1);




	GLfloat tempTriangle[]={-1,0,0,0,1,0,1,0,0};
	
	cnmgrInit();

	//cnmgrCreateFragShaderFromFile(&fshader,"fshader");
	//cnmgrCreateVertexShaderFromFile(&vshader,"vshader");
	//cnmgrCreateGLProgramFromFragmentShader(&tempProgram,&fshader,0);

	cnmgrAddCameraSceneNode(&camera,65,0.01f,100);
	myTriangle=cnmgrAddMeshNodeFromNonIndexedFloatArray(&camera,0,NULL,0,3,9,tempTriangle);
	//myTriangle->shaderProgram=&tempProgram;
}

loopThing()
{
	int width;
	int height;
	double time;//lol
	float fps;
	

	while (!glfwWindowShouldClose(window))
	{
		time=glfwGetTime();
		glfwGetWindowSize(window,&width,&height);
		glViewport(0,0,width,height);
		glClearColor(0,1,0,0);
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
		{
			myTriangle->position[1]=myTriangle->position[1]+0.1f;
		}

		if (glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
		{
			myTriangle->position[1]=myTriangle->position[1]-0.1f;
		}
		if (glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
		{
			myTriangle->position[0]=myTriangle->position[0]-0.1f;
		}
		if (glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
		{
			myTriangle->position[0]=myTriangle->position[0]+0.1f;
		}
		if (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS)
		{
			myTriangle->position[2]=myTriangle->position[2]-0.1f;
		}
		if (glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
		{
			myTriangle->position[2]=myTriangle->position[2]+0.1f;
		}

		if (glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS)
		{
			printf("%f pos\n",myTriangle->position[2]);
		}


		cnmgrRenderScene(&camera,width,height);

		glfwSwapBuffers(window);
		glfwPollEvents();

		fps=1/(glfwGetTime()-time);
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