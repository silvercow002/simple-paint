#include<stdio.h>
#include<stdlib.h>
#include<GL/freeglut.h>

#include"menu.h"



// TODO:
//  change this parameter to argv
int width, height;


void display(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  fprintf(stderr, "display() is called\n");
}

int main(int argc, char** argv) {

  // glut init
  glutInit(&argc, argv);
  // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // performancnce frist
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

  glutInitWindowSize(width ? width : 800,  height ? height : 800);
  glutInitWindowPosition(100, 50);

  
  glutCreateWindow("hw1");

  menu_init();
  
  glutDisplayFunc(display);




  glutMainLoop();
}