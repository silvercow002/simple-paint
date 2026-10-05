#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>

#include"draw.h"
#include"pen.h"

// circles data
typedef struct {
    size_t count;
    size_t capacity;
    struct {
        int x;
        int y;
        float radius;
    } meta[];
} Circles;



static int first;
static int pos_x, pos_y;
static Circles *circles;


static void ds_init() {
    pos_x = -1, pos_y = -1;

    
    size_t _size = 1000;
    circles = malloc(
        sizeof *circles +
        _size * sizeof circles->meta[0]
    );
    circles->capacity = _size;
    circles->count = 0;

}



void draw_polygon() {
    int i;
    
}


static void draw_circle() {
    fprintf(stderr, "draw_circle() was benn clike\n");
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
            16,
            3
        );
        glPopMatrix();
    }

    
}


int draw_mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
        return 0;
    }

    switch (shape) {
    case SHAPE_NONE:
        // do nothing
        break;
    case SHAPE_LINE:
        if (!first) {
            first = 1;            
            pos_x = x;
            pos_y = y;
            glPointSize(pnt_size);
            glBegin(GL_POINTS);
            glVertex3f(x, height-y, 0);
            glEnd();
        }
        else {
            first = 0;
            glLineWidth(pnt_size);
            glBegin(GL_LINES);
            glVertex2f(pos_x, height-pos_y);
            glVertex2f(x, height-y);
            glEnd();
        }
        break;

    case SHAPE_MISUMI:
        break;
    case SHAPE_CIRCLE:
        if (circles->count <= circles->capacity) {
            circles->meta[circles->count].x = x;
            circles->meta[circles->count].y = y;
            circles->meta[circles->count].radius = 15.0;
            circles->count++;
        }
        break;
    default:
        break;
    }
    glFinish();
    return 1;
}

int draw_motion(int x, int y) {
    if (brush == BRUSH_NONE) {
        return 0;
    }
    
    if (first == 0) {
        first = 1;
        pos_x = x;
        pos_y = y;
        glPointSize(pnt_size); 
        glBegin(GL_POINTS);
        glVertex3f(x, height-y, 0);
        glEnd();
    }
    
    else {
        
    }
    
    return 1;
}

void draw_render() {
    // circle
    draw_circle();
}

void draw_init(){
    ds_init();
}