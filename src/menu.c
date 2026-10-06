#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include"menu.h"
#include"pen.h"
static void (*tool_changed)(void);

Brush_e brush = BRUSH_DOT;
Shape_e shape = SHAPE_NONE;
float pnt_size = 1.0f;
float line_width = 1.0f;
color_e pnt_color = white;

static int brush_menu, shape_menu, color_menu;
static int width_menu, point_menu, main_menu;

typedef struct {
    const char *label;
    int value;
} MenuOption;

static const MenuOption brush_options[] = {
    {"Dot", BRUSH_DOT}, {"Square", BRUSH_SQUARE}
};
static const MenuOption shape_options[] = {
    {"None", SHAPE_NONE}, {"Line", SHAPE_LINE},
    {"Triangle", SHAPE_MISUMI}, {"Circle", SHAPE_CIRCLE}
};
static const MenuOption color_options[] = {
    {"Red", red}, {"Blue", blue}, {"Green", green},
    {"Yellow", yellow}, {"White", white}, {"Black", black}
};
static const MenuOption size_options[] = {
    {"1 px", 1}, {"2 px", 2}, {"4 px", 4},
    {"8 px", 8}, {"16 px", 16}
};

#define OPTION_COUNT(options) (sizeof(options) / sizeof((options)[0]))

static void refresh_group(int menu, int parent_entry, const char *title,
                          const MenuOption *options, size_t count, int selected) {
    char label[80];
    const char *selected_label = "None";
    glutSetMenu(menu);
    for (size_t i = 0; i < count; ++i) {
        int active = options[i].value == selected;
        snprintf(label, sizeof label, "%s %s", active ? "[x]" : "[ ]",
                 options[i].label);
        glutChangeToMenuEntry((int)i + 1, label, options[i].value);
        if (active) selected_label = options[i].label;
    }
    snprintf(label, sizeof label, "%s: %s", title, selected_label);
    glutSetMenu(main_menu);
    glutChangeToSubMenu(parent_entry, label, menu);
}

static void refresh_selection(void) {
    int previous_menu = glutGetMenu();
    refresh_group(color_menu, 1, "Color", color_options,
                  OPTION_COUNT(color_options), pnt_color);
    refresh_group(width_menu, 2, "Line Width", size_options,
                  OPTION_COUNT(size_options), (int)line_width);
    refresh_group(point_menu, 3, "Point Size", size_options,
                  OPTION_COUNT(size_options), (int)pnt_size);
    refresh_group(brush_menu, 4, "Brush", brush_options,
                  OPTION_COUNT(brush_options), brush);
    refresh_group(shape_menu, 5, "Shape", shape_options,
                  OPTION_COUNT(shape_options), shape);
    glutSetMenu(previous_menu);
}


static void parent_callback(int value) {
    if (value == -1) {
        exit(0);
    } 
}

static void brush_callback(int choice) {
    switch (choice)
    {
        case BRUSH_DOT:
            brush = BRUSH_DOT;
            if (tool_changed) tool_changed();
            fprintf(stderr, "menu 'brush' is being tap\n");
            break;
        case BRUSH_SQUARE:
            brush = BRUSH_SQUARE;
            if (tool_changed) tool_changed();
            fprintf(stderr, "menu 'shape' is being tap\n");
            break;
    }
    refresh_selection();
}

static void shape_callback(int choice) {
    if (tool_changed) tool_changed();
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
    refresh_selection();
}

static void color_callback(int choice) {
    if (choice >= red && choice <= black) pnt_color = (color_e)choice;
    refresh_selection();
}

static void line_width_callback(int choice) {
    line_width = (float)choice;
    refresh_selection();
}
static void point_size_callback(int choice) {
    pnt_size = (float)choice;
    refresh_selection();
}

static int size_menu(void (*callback)(int)) {
    int menu = glutCreateMenu(callback);
    glutAddMenuEntry("1 px", 1);
    glutAddMenuEntry("2 px", 2);
    glutAddMenuEntry("4 px", 4);
    glutAddMenuEntry("8 px", 8);
    glutAddMenuEntry("16 px", 16);
    return menu;
}

void menu_init(void (*on_tool_changed)(void))
{
    tool_changed = on_tool_changed;
    brush_menu = glutCreateMenu(brush_callback);
    glutAddMenuEntry("Dot", BRUSH_DOT);
    glutAddMenuEntry("Square", BRUSH_SQUARE);

    shape_menu = glutCreateMenu(shape_callback);
    glutAddMenuEntry("None", SHAPE_NONE);
    glutAddMenuEntry("Line", SHAPE_LINE);
    glutAddMenuEntry("Triangle", SHAPE_MISUMI);
    glutAddMenuEntry("Circle", SHAPE_CIRCLE);

    color_menu = glutCreateMenu(color_callback);
    glutAddMenuEntry("Red", red);
    glutAddMenuEntry("Blue", blue);
    glutAddMenuEntry("Green", green);
    glutAddMenuEntry("Yellow", yellow);
    glutAddMenuEntry("White", white);
    glutAddMenuEntry("Black", black);
    width_menu = size_menu(line_width_callback);
    point_menu = size_menu(point_size_callback);

    main_menu = glutCreateMenu(parent_callback);
    glutAddSubMenu("Brush", brush_menu);
    glutAddSubMenu("Shape", shape_menu);
    glutAddSubMenu("Color", color_menu);
    glutAddSubMenu("Line Width", width_menu);
    glutAddSubMenu("Point Size", point_menu);
    glutAddMenuEntry("Quit", -1);


    refresh_selection();
    glutSetMenu(main_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}