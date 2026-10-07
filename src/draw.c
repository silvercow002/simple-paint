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

typedef struct {
    size_t count;
    size_t capacity;
    Point meta[];
} Points;

typedef struct {
    int x, y;
} Vertex;

typedef struct {
    Vertex vertices[3];
    color_e color;
    float width;
    int outline;
} Triangle;

typedef struct {
    size_t count;
    size_t capacity;
    Triangle meta[];
} Triangles;
static Vertex misumi_vertices[3];
static int misumi_vertex_count;



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
static Points *points;
static Lines *lines;
static Circles *circles;
static Triangles *misumis;
static GLUquadric *disk_quadric;

static void free_disk(void) {
    if (disk_quadric) gluDeleteQuadric(disk_quadric);
    disk_quadric = NULL;
}


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

    misumis = malloc(
        sizeof *misumis +
        _size * sizeof misumis->meta[0]
    );
    misumis->count = 0;
    misumis->capacity = _size;

    points = malloc(
        sizeof *points +
        _size * sizeof points->meta[0]
    );
    points->count = 0;
    points->capacity = _size;
}

void draw_string(float x, float y, const char* str) {
    glRasterPos2f(x, y);
    for (const char* i = str; *i != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
    
}

static void apply_color(color_e color) {
    static const GLfloat colors[][3] = {
        {1, 0, 0}, {0, 0, 1}, {0, 1, 0},
        {1, 1, 0}, {0, 0, 0}
    };
    glColor3fv(colors[color]);
}

static void draw_triangles(void)
{
    glPushAttrib(GL_LINE_BIT);
    for (size_t i = 0; i < misumis->count; ++i) {
        const Triangle *misumi = &misumis->meta[i];
        apply_color(misumi->color);
        if (misumi->outline) glLineWidth(misumi->width);
        glBegin(misumi->outline ? GL_LINE_LOOP : GL_TRIANGLES);
        for (int j = 0; j < 3; ++j) {
            glVertex2i(misumi->vertices[j].x,
                       height - misumi->vertices[j].y);
        }
        glEnd();
    }
    glPopAttrib();
}

static void draw_points(void) {
    for (size_t i = 0; i < points->count; ++i) {
        const Point *p = &points->meta[i];
        apply_color(p->color);
        float r = p->size * 0.5f;
        float cx = (float)p->x, cy = (float)(height - p->y);
        if (p->brush == BRUSH_SQUARE) {
            glBegin(GL_QUADS);
            glVertex2f(cx-r, cy-r); glVertex2f(cx+r, cy-r);
            glVertex2f(cx+r, cy+r); glVertex2f(cx-r, cy+r);
            glEnd();
        } else {
            glPushMatrix();
            glTranslatef(cx, cy, 0.0f);
            gluDisk(disk_quadric, 0.0, r, 32, 1);
            glPopMatrix();
        }
    }
}

void draw_cancel_input(void) {
    misumi_vertex_count = 0;
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
    for (size_t i=0; i < circles->count; ++i) {
        apply_color(circles->meta[i].color);
        glPushMatrix();
        glTranslatef(
            circles->meta[i].x,
            height - circles->meta[i].y,
            0.0f
        );
        gluDisk(disk_quadric,
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
            if (points->count >= points->capacity) {
                fprintf(stderr, "points reached capacity\n");
                break;
            }
            points->meta[points->count++] = (Point){x, y, pnt_size, pnt_color, brush};
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
    case SHAPE_MISUMI_OUTLINE:
        freehand = 0;
        if (misumis->count >= misumis->capacity) {
            fprintf(stderr, "misumis reached capacity\n");
            break;
        }
        misumi_vertices[misumi_vertex_count++] = (Vertex){x, y};
        fprintf(stderr, "Triangle vertex: %d/3\n", misumi_vertex_count);
        if (misumi_vertex_count == 3) {
            misumis->meta[misumis->count++] = (Triangle){
                {misumi_vertices[0], misumi_vertices[1], misumi_vertices[2]},
                pnt_color, line_width, shape == SHAPE_MISUMI_OUTLINE
            };
            misumi_vertex_count = 0;
            glutPostRedisplay();
        }
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
        if (points->count >= points->capacity) {
            freehand = 0;
            fprintf(stderr, "points reached capacity\n");
            glutPostRedisplay();
            return 0;
        }
        float t = (float)i / steps;
        int px = (int)lroundf(last_x + dx * t);
        int py = (int)lroundf(last_y + dy * t);
        points->meta[points->count++] = (Point){px, py, pnt_size, pnt_color, brush};
    }
    last_x = x;
    last_y = y;

    glutPostRedisplay();
    return 1;
}

void draw_render() {
    draw_line();
    draw_circle();
    draw_triangles();
    draw_points();
}

void draw_init(){
    disk_quadric = gluNewQuadric();
    gluQuadricDrawStyle(disk_quadric, GLU_FILL);
    atexit(free_disk);
    ds_init();
}

void draw_clear_saved(void)
{
    misumis->count = 0;
    lines->count = 0;
    circles->count = 0;
    points->count = 0;
}
