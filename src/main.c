#include<stdio.h>
#include<stdlib.h>
#include<GL/freeglut.h>

#include"menu.h"
#include"draw.h"



// TODO:
//  change this parameter to argv
int width, height;


static void display(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  fprintf(stderr, "display() is called\n");
}


static void keybord_handler(unsigned char key, int x, int y) {
  if (draw_keybord(key, x, y)) {
    return;
  }

  switch (key) {
  case 'Q':
    exit(0);
    break;
  case 'q':
    exit(0);
    break;
  default:
    break;
  }
}


// static void mouse_handler(int button, int state, int x, int y) {
//   // if ()
// }


// static void motion_handler(int x, int y) {

// }

int main(int argc, char** argv) {

  // glut init
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // performancnce frist

  glutInitWindowSize(width ? width : 800,  height ? height : 800);
  glutInitWindowPosition(100, 50);

  
  glutCreateWindow("hw1");

  menu_init();
  
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keybord_handler);
  // glutMouseFunc(mouse_handler);
  // glutMotionFunc(motion_handler);




  glutMainLoop();
}