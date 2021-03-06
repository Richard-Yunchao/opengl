/*


  Simple Demo for GLSL

  www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"


GLuint v,f,f2,p;
// float lpos[4] = {1,0.5,1,0};
GLint attribute_coord2d;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}
float a = 0;

void renderScene(void) {
/* Clear the background as white */ 
  glClearColor(1.0, 1.0, 1.0, 1.0); 
  glClear(GL_COLOR_BUFFER_BIT); 
   
  glUseProgram(p); 
  glEnableVertexAttribArray(attribute_coord2d); 
  GLfloat triangle_vertices[] = { 
     0.0,  0.8, 
    -0.8, -0.8, 
     0.8, -0.8, 
  }; 
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */ 
  glVertexAttribPointer( 
    attribute_coord2d, // attribute  
    2,                 // number of elements per vertex, here (x,y)  
    GL_FLOAT,          // the type of each element  
    GL_FALSE,          // take our values as-is  
    0,                 // no extra data between each position  
    triangle_vertices  // pointer to the C array  
  ); 
   
  /* Push each element in buffer_vertices to the vertex shader */ 
  glDrawArrays(GL_TRIANGLES, 0, 3); 
  glDisableVertexAttribArray(attribute_coord2d); 
   
  /* Display the result */ 
  glutSwapBuffers();  
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}


void setShaders() {

	char *vs = NULL,*fs = NULL ,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	f2 = glCreateShader(GL_FRAGMENT_SHADER);


	vs = textFileRead("tri.vs");
	fs = textFileRead("tri.fs");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	printShaderInfoLog(f2);

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	printProgramInfoLog(p);

/*	glUseProgram(p);

	GLint loc1,loc2,loc3,loc4;
	float specIntensity = 0.98;
	float sc[4] = {0.8,0.8,0.8,1.0};
	float threshold[2] = {0.5,0.25};
	float colors[12] = {0.4,0.4,0.8,1.0,
						0.2,0.2,0.4,1.0,
						0.1,0.1,0.1,1.0};

	loc1 = glGetUniformLocation(p,"specIntensity");
	glUniform1f(loc1,specIntensity);

	loc2 = glGetUniformLocation(p,"specColor");
//	glUniform4fv(loc2,1,sc);
	glUniform4f(loc2,sc[0],sc[1],sc[2],sc[3]);
	loc3 = glGetUniformLocation(p,"t");
	glUniform1fv(loc3,2,threshold);

	loc4 = glGetUniformLocation(p,"colors");
	glUniform4fv(loc4,3,colors);
*/
}




int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("MM 2004-05");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	// glEnable(GL_DEPTH_TEST);               // why this prog can't enable depth test? 
	glClearColor(1.0,1.0,1.0,1.0);
//	glEnable(GL_CULL_FACE);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	setShaders();

	glutMainLoop();

	return 0;
}

