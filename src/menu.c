#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include"menu.h"
#include"pen.h"

Brush_e brush = BRUSH_DOT;
Shape_e shape = SHAPE_NONE;
float pnt_size = 1.0f;

static void parent_callback(int value) {
    if (value == -1) {
        exit(0);
    } 
}

static void brush_callback(int choice) {
    switch (choice)
    {
        case BRUSH_DOT:
            fprintf(stderr, "menu 'brush' is being tap\n");
            break;
        case BRUSH_SQUARE:
            fprintf(stderr, "menu 'shape' is being tap\n");
            break;
    }
}

static void shape_callback(int choice) {
    switch(choice) {
    case SHAPE_NONE:
        shape = SHAPE_NONE;    
        fprintf(stderr, "shape is set to 'None'\n");
        break;
    case SHAPE_LINE:
        shape = SHAPE_LINE;
        fprintf(stderr, "shape is set to 'Line'\n");
        break;
    case SHAPE_MISUMI:
        shape = SHAPE_MISUMI;
        fprintf(stderr, "shape is set to 'MISUMI'\n");
        break;
    case SHAPE_CIRCLE:
        shape = SHAPE_CIRCLE;
        fprintf(stderr, "shape is set to 'Circle'\n");
        break;
    }
}

static void color_callback(int choice) {
    
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
    glutAddMenuEntry("Circle", SHAPE_CIRCLE);

    int main_menu = glutCreateMenu(parent_callback);
    glutAddSubMenu("Brush", brush_menu);
    glutAddSubMenu("Shape", shape_menu);
    glutAddMenuEntry("Quit", -1);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}