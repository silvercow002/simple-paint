#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>

#include"draw.h"
#include"pen.h"

typedef struct {
    int x, y;
    float size;
    color_e color;
    Brush_e brush;
} Point;

static Point *points;
static size_t point_count, point_capacity;


typedef struct {
    int x1, y1;
    int x2, y2;
    float width;
    color_e color;
} Line;

typedef struct {
    size_t count;
    size_t capacity;
    Line meta[];
} Lines;

typedef struct {
    int x, y;
    float radius;
    color_e color;
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
static Circles *circles;


static void ds_init() {
    pos_x = -1, pos_y = -1;

    
    size_t _size = 10000;
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

    point_capacity = _size;
    points = malloc(point_capacity * sizeof *points);
}

void draw_string(float x, float y, const char* str) {
    glRasterPos2f(x, y);
    for (const char* i = str; *i != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
    
}

void draw_polygon() {
    int i;
    
}

static void apply_color(color_e color) {
    static const GLfloat colors[][3] = {
        {1, 0, 0}, {0, 0, 1}, {0, 1, 0},
        {1, 1, 0}, {1, 1, 1}, {0, 0, 0}
    };
    glColor3fv(colors[color]);
}

static void draw_points(void) {
    for (size_t i = 0; i < point_count; ++i) {
        const Point *p = &points[i];
        apply_color(p->color);
        float r = p->size * 0.5f;
        float cx = (float)p->x, cy = (float)(height - p->y);
        if (p->brush == BRUSH_SQUARE) {
            glBegin(GL_QUADS);
            glVertex2f(cx-r, cy-r); glVertex2f(cx+r, cy-r);
            glVertex2f(cx+r, cy+r); glVertex2f(cx-r, cy+r);
            glEnd();
        } else {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for (int j = 0; j <= 32; ++j) {
                float a = j * 6.283185307f / 32;
                glVertex2f(cx + r*cosf(a), cy + r*sinf(a));
            }
            glEnd();
        }
    }
}

void draw_cancel_input(void) {
    freehand = 0;
    pending = 0;
}

static void draw_line() {
    for (size_t i=0; i < lines->count; ++i) {
        Line *tmp = &lines->meta[i];
        apply_color(tmp->color);
        glLineWidth(tmp->width);

        glBegin(GL_LINES);
        glVertex2i(tmp->x1, height-tmp->y1);
        glVertex2i(tmp->x2, height-tmp->y2);
        glEnd();
    }
}

static void draw_circle() {
    fprintf(stderr, "draw_circle() was benn clike\n");
    static GLUquadric *_circle = NULL;
    
    if (_circle == NULL) {
        _circle = gluNewQuadric();
        gluQuadricDrawStyle(_circle, GLU_FILL);
    }


    for (size_t i=0; i < circles->count; ++i) {
        apply_color(circles->meta[i].color);
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
        if (freehand) draw_motion(x, y);
        freehand = 0;
        return 1;
    }

    switch (shape) {
    case SHAPE_NONE:
        if (brush == BRUSH_DOT || brush == BRUSH_SQUARE) {
            if (point_count >= point_capacity) {
                fprintf(stderr, "points reached capacity\n");
                break;
            }
            points[point_count++] = (Point){x, y, pnt_size, pnt_color, brush};
            glutPostRedisplay();
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
                fprintf(stderr, "circles instance is reach the limits\n");
                break;
            }
            lines->meta[lines->count] = (Line){
                pos_x, pos_y,
                x, y,
                line_width, pnt_color
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
            fprintf(stderr, "circles instance is reach the limits\n");
            break;
        }
        circles->meta[circles->count] = (Circle){
            x, y, 15.0, pnt_color
        };
        circles->count++;
        glutPostRedisplay();
        break;
    }
    return 1;
}

int draw_motion(int x, int y) {
    if (!freehand ||
        shape != SHAPE_NONE ||
        (brush != BRUSH_DOT && brush != BRUSH_SQUARE)
    )  {
        return 0;
    }

    if (x == last_x && y == last_y) {
        return 1;
    }

    float dx = (float)x - last_x;
    float dy = (float)y - last_y;
    float distance = sqrtf(dx * dx + dy * dy);

    // interpolation
    float spacing = fmaxf(1.0f, pnt_size * 0.25f);
    int steps = (int)ceilf(distance / spacing);

    for (int i = 1; i <= steps; ++i) {
        if (point_count >= point_capacity) {
            freehand = 0;
            fprintf(stderr, "points reached capacity\n");
            glutPostRedisplay();
            return 0;
        }
        float t = (float)i / steps;
        int px = (int)lroundf(last_x + dx * t);
        int py = (int)lroundf(last_y + dy * t);
        points[point_count++] = (Point){px, py, pnt_size, pnt_color, brush};
    }
    last_x = x;
    last_y = y;

    glutPostRedisplay();
    return 1;
}

void draw_render() {
    draw_line();
    draw_circle();
    draw_points();
}

void draw_init(){
    ds_init();
}

void draw_clear_saved(void)
{
    lines->count = 0;
    circles->count = 0;
    point_count = 0;
}
