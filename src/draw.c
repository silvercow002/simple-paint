#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>

#include"draw.h"
#include"pen.h"


typedef struct {
    int x1, y1;
    int x2, y2;
    float width;
} Line;

typedef struct {
    size_t count;
    size_t capacity;
    Line meta[];
} Lines;

typedef struct {
    int x, y;
    float radius;
} Circle;

typedef struct {
    size_t count;
    size_t capacity;
    Circle meta[];
} Circles;



// as bool
static int freehand, pending;
// last postion
static int pos_x, pos_y;
static int last_x, last_y;

// store
static Lines *lines;
static Lines *curve;
static Circles *circles;


static void ds_init() {
    pos_x = -1, pos_y = -1;

    
    size_t _size = 1000000;
    circles = malloc(
        sizeof *circles +
        _size * sizeof circles->meta[0]
    );
    circles->count = 0;
    circles->capacity = _size;

    lines = malloc(
        sizeof *lines +
        _size * sizeof lines->meta[0]
    );
    lines->count = 0;
    lines->capacity = _size;

    curve = malloc(
        sizeof *curve +
        _size * sizeof curve->meta[0]
    );
    curve->count = 0;
    curve->capacity = _size;
}



void draw_polygon() {
    int i;
    
}

static void draw_line() {
    for (size_t i=0; i < lines->count; ++i) {
        Line *tmp = &lines->meta[i];
        glLineWidth(tmp->width);

        glBegin(GL_LINES);
        glVertex2i(tmp->x1, height-tmp->y1);
        glVertex2i(tmp->x2, height-tmp->y2);
        glEnd();
    }
}

static void draw_curve() {
    for (size_t i=0; i < curve->count; ++i) {
        Line *tmp = &curve->meta[i];
        glLineWidth(tmp->width);

        glBegin(GL_LINES);
        glVertex2i(tmp->x1, height-tmp->y1);
        glVertex2i(tmp->x2, height-tmp->y2);
        glEnd();
    }
}

static void draw_circle() {
    fprintf(stderr, "draw_circle() was benn clike");
    static GLUquadric *_circle = NULL;
    
    if (_circle == NULL) {
        _circle = gluNewQuadric();
        gluQuadricDrawStyle(_circle, GLU_FILL);
    }


    for (size_t i=0; i < circles->count; ++i) {
        glPushMatrix();
        glTranslatef(
            circles->meta[i].x,
            height - circles->meta[i].y,
            0.0f
        );
        gluDisk(_circle,
            0.0,
            circles->meta[i].radius,
            128,
            3
        );
        glPopMatrix();
    }
}


int draw_mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON) {
        return 0;
    }

    if (state == GLUT_UP) {
        freehand = 0;
        return 1;
    }

    switch (shape) {
    case SHAPE_NONE:
        if (brush == BRUSH_DOT) {
            pending = 0;
            freehand = 1;
            last_x = x;
            last_y = y;
        }
        break;
    case SHAPE_LINE:
        freehand = 0;
        if (!pending) {
            pos_x = x, pos_y = y;
            pending = !pending;
        }
        else {
            if (lines->count >= lines->capacity) {
                fprintf(stderr, "circles instance is reach the limits");
                break;
            }
            lines->meta[lines->count] = (Line){
                pos_x, pos_y,
                x, y,
                pnt_size
            };
            lines->count++;
            pending = !pending;
            glutPostRedisplay();
        }
        break;

    case SHAPE_MISUMI:
        break;
    case SHAPE_CIRCLE:
        if (circles->count >= circles->capacity) {
            fprintf(stderr, "circles instance is reach the limits");
            break;
        }
        circles->meta[circles->count] = (Circle){
            x, y, 15.0
        };
        circles->count++;
        glutPostRedisplay();
        break;
    }
    glFinish();
    return 1;
}

int draw_motion(int x, int y) {
    if (!freehand ||
        shape != SHAPE_NONE ||
        brush != BRUSH_DOT
    )  {
        return 0;
    }

    if (x == last_x && y == last_y) {
        return 1;
    }

    if (curve->count >= curve->capacity) {
        freehand = 0;
        fprintf(stderr, "curve instance is reach the limits");
        return 0;
    }

    curve->meta[curve->count] = (Line){
        last_x, last_y, x, y, pnt_size
    };
    curve->count++; 
    last_x = x;
    last_y = y;

    glutPostRedisplay();
    return 1;
}

void draw_render() {
    draw_line();
    draw_circle();
    draw_curve();
}

void draw_init(){
    ds_init();
}