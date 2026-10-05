#include<stdio.h>
#include<stdlib.h>
#include<GL/freeglut.h>

#include"menu.h"
#include"draw.h"

// TODO:
//  change this parameter to argv
int width, height;

void init_window(void)
{
    // set matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // size
    // set left-down(0, 0), right-up(w, h)
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);

    // all windows as painting area
    glViewport(0, 0, width, height);

    // reset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDrawBuffer(GL_BACK);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutPostRedisplay();
}

static void init_func()
{
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_BACK);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    draw_render();

    glutSwapBuffers();
    fprintf(stderr, "\n");
}

static void reshape(int nw, int nh) {
    width = nw;
    height = nh;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}


static void keybord_handler(unsigned char key, int x, int y) {

    switch (key) {
    case 'Q':
        exit(0);
        break;
    case 'q':
        exit(0);
        break;
    case 'C':
        break;
    case 'c':
        break;
    default:
        break;
    }
}

static void motion_handler(int x, int y) {
    if (draw_motion(x, y)) {
        return;
    }
    
}


static void mouse_handler(int button, int state, int x, int y) {
    if (draw_mouse(button, state, x, y)) {
        return;
    }

}


int main(int argc, char** argv) {

    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // performancnce frist

    glutInitWindowSize(width ? width : 400,  height ? height : 400);
    // glutInitWindowPosition(100, 50);


    glutCreateWindow("hw1");

    menu_init(draw_cancel_input);
    draw_init();

    init_window();
    init_func();

    glutDisplayFunc(display);
    glutKeyboardFunc(keybord_handler);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);




    glutMainLoop();
}