#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include"menu.h"
#include"pen.h"

Brush_e brush = BRUSH_DOT;
Shape_e shape = SHAPE_NONE;

static void parent_callback(int value) {
    if (value == -1) {
        exit(0);
    } 
}

static void brush_callback(int choice) {
    switch (choice)
    {
        case BRUSH_DOT:
            fprintf(stdout, "menu 'brush' is being tap");
            break;
        case BRUSH_SQUARE:
            fprintf(stdout, "menu 'shape' is being tap");
            break;
        // case MENU_QUIT:
        //     fprintf(stdout, "normal quit\n");
        //     exit(0);
        //     break;
    }
}

static void shape_callback(int choice) {
    switch(choice) {
        case SHAPE_NONE:
            break;
        case SHAPE_LINE:
            break;
        case SHAPE_MISUMI:
            break;
    }
}

void menu_init(void)
{
    int brush_menu = glutCreateMenu(brush_callback);
    glutAddMenuEntry("Dot", BRUSH_DOT);
    glutAddMenuEntry("Square", BRUSH_SQUARE);

    int shape_menu = glutCreateMenu(shape_callback);
    glutAddMenuEntry("None", SHAPE_NONE);
    glutAddMenuEntry("Line", SHAPE_LINE);
    glutAddMenuEntry("Triangle", SHAPE_MISUMI);

    int main_menu = glutCreateMenu(parent_callback);
    glutAddSubMenu("Brush", brush_menu);
    glutAddSubMenu("Shape", shape_menu);
    glutAddMenuEntry("Quit", -1);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}