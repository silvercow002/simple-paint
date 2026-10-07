#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>

#include "Mrwithe.h"
#include "draw.h"
#include "type.h"
#include "canvas_io.h"

typedef struct {
    int x, y;
} EraserPoint;

typedef struct {
    size_t count;
    size_t capacity;
    EraserPoint meta[];
} EraserPoints;

static EraserPoints *points;
static int active, pressed, visible;
static int mouse_x, mouse_y;
static const float radius = 15.0f;
static GLUquadric *disk_quadric;
static int *snapshot_ready;


static void ds_init(void) {
    size_t _size = 10000;
    points = malloc(
        sizeof *points +
        _size * sizeof points->meta[0]
    );
    points->count = 0;
    points->capacity = _size;
}


static void disk(int x, int y, int height) {
    if (!disk_quadric) {
        disk_quadric = gluNewQuadric();
        gluQuadricDrawStyle(disk_quadric, GLU_FILL);
    }
    glPushMatrix();
    glTranslatef((float)x, (float)(height - y), 0.0f);
    gluDisk(disk_quadric, 0.0, radius, 48, 1);
    glPopMatrix();
}

static void append(int x, int y) {
    if (points->count == points->capacity) {
        size_t next = points->capacity * 2;
        EraserPoints *tmp = realloc(points,
            sizeof *points + next * sizeof points->meta[0]);
        points = tmp;
        points->capacity = next;
    }
    points->meta[points->count++] = (EraserPoint){x, y};
}

void eraser_begin(void) {
    eraser_capture();
    draw_cancel_input();
    type_cancel();
    active = 1;
    pressed = visible = 0;
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPostRedisplay();
}

void eraser_end(void) {
    eraser_capture();
    active = pressed = visible = 0;
    glutSetCursor(GLUT_CURSOR_INHERIT);
    glutPostRedisplay();
}

int eraser_keyboard(unsigned char key) {
    if (!active || key != 27) return 0;
    eraser_end();
    return 1;
}


// hold left mouse button to erase
// Esc to exit
void eraser_motion(int x, int y) {
    if (!active) {
        draw_motion(x, y);
        return;
    }
    if (pressed) {
        // interpolation
        float dx = (float)x - mouse_x, dy = (float)y - mouse_y;
        int steps = (int)ceilf(sqrtf(dx * dx + dy * dy) / (radius * 0.25f));
        for (int i = 1; i <= steps; ++i) {
            float t = (float)i / steps;
            append((int)lroundf(mouse_x + dx * t), (int)lroundf(mouse_y + dy * t));
        }
    }
    mouse_x = x;
    mouse_y = y;
    visible = 1;
    glutPostRedisplay();
}

int eraser_mouse(int button, int state, int x, int y) {
    if (!active) return 0;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_x = x;
            mouse_y = y;
            pressed = visible = 1;
            append(x, y);
        } else {
            eraser_motion(x, y);
            pressed = 0;
        }
        glutPostRedisplay();
    }
    return 1;
}

static void eraser_entry(int state) {
    if (!active) return;
    visible = state == GLUT_ENTERED;
    if (!visible) pressed = 0;
    glutPostRedisplay();
}

static void eraser_render(int height) {
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1, 1, 1);
    for (size_t i = 0; i < points->count; ++i)
        disk(points->meta[i].x, points->meta[i].y, height);
    glPopAttrib();
}

void eraser_preview(int height) {
    if (!active || !visible || pressed) return;
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.6f, 0.6f, 0.6f);
    disk(mouse_x, mouse_y, height);
    glPopAttrib();
}

static void eraser_free(void) {
    if (disk_quadric) gluDeleteQuadric(disk_quadric);
    disk_quadric = NULL;
    free(points);
    points = NULL;
}

void eraser_capture(void) {
    if (points->count == 0) return;
    glClear(GL_COLOR_BUFFER_BIT);
    if (*snapshot_ready) canvas_restore(width, height);
    draw_render();
    type_render(height);
    eraser_render(height);
    canvas_capture(width, height);
    *snapshot_ready = 1;
    draw_clear_saved();
    type_clear_saved();
    points->count = 0;
}

void eraser_init(int *ready) {
    ds_init();
    snapshot_ready = ready;
    glutPassiveMotionFunc(eraser_motion);
    glutMotionFunc(eraser_motion);
    glutEntryFunc(eraser_entry);
    atexit(eraser_free);
}