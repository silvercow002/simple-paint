#include <stdio.h>
#include <GL/freeglut.h>

#include "overlay.h"

static int grid_visible;
static int cur_time, last_time, fps_counter;
static float fps;

static void FPS() {
    fps_counter++;
    cur_time = glutGet(GLUT_ELAPSED_TIME);
    int interval = cur_time - last_time;
    if (interval >= 500) {
        fps = fps_counter * 500 / interval;
        last_time = cur_time;
        fps_counter = 0;
        fprintf(stderr, "FPS: %.1f\n", fps);
    }
}


static void draw_grid(int width, int height) {
    const int minor = 10, major = 50;
    int cx = width / 2, cy = height / 2;
    void *font = GLUT_BITMAP_HELVETICA_10;
    char label[32];

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    for (int x = cx % minor; x < width; x += minor) {
        if (x == cx) continue;
        glColor4f(0.25f, 0.30f, 0.38f,
                  (x - cx) % major == 0 ? 0.45f : 0.18f);
        glVertex2i(x, 0); glVertex2i(x, height);
    }
    for (int y = cy % minor; y < height; y += minor) {
        if (y == cy) continue;
        glColor4f(0.25f, 0.30f, 0.38f,
                  (y - cy) % major == 0 ? 0.45f : 0.18f);
        glVertex2i(0, y); glVertex2i(width, y);
    }
    glEnd();

    glColor4f(0.15f, 0.18f, 0.22f, 0.9f);
    glBegin(GL_LINES);
    glVertex2i(0, cy); glVertex2i(width, cy);
    glVertex2i(cx, 0); glVertex2i(cx, height);
    for (int x = cx % major; x < width; x += major) {
        glVertex2i(x, cy - 3); glVertex2i(x, cy + 3);
    }
    for (int y = cy % major; y < height; y += major) {
        glVertex2i(cx - 3, y); glVertex2i(cx + 3, y);
    }
    glEnd();

    for (int x = cx % major; x < width; x += major) {
        if (x == cx) continue;
        snprintf(label, sizeof label, "%d", x - cx);
        int text_width = glutBitmapLength(font, (const unsigned char *)label);
        int left = x - text_width / 2;
        if (left < 0 || left + text_width >= width || cy < 14) continue;
        glRasterPos2i(left, cy - 14);
        glutBitmapString(font, (const unsigned char *)label);
    }
    for (int y = cy % major; y < height - 12; y += major) {
        if (y == cy) continue;
        snprintf(label, sizeof label, "%d", y - cy);
        glRasterPos2i(cx + 6, y + 3);
        glutBitmapString(font, (const unsigned char *)label);
    }
    glRasterPos2i(cx + 6, cy + 5);
    glutBitmapString(font, (const unsigned char *)"0");
    glRasterPos2i(width - 12, cy + 6);
    glutBitmapString(font, (const unsigned char *)"x");
    glRasterPos2i(cx + 6, height - 12);
    glutBitmapString(font, (const unsigned char *)"y");
    glPopAttrib();
}

void overlay_init(void) {
    grid_visible = 0;
    fps_counter = 0;
    fps = 0.0f;
    last_time = glutGet(GLUT_ELAPSED_TIME);
}

void overlay_toggle_grid(void) {
    grid_visible = !grid_visible;
    fprintf(stderr, "Grid: %s\n", grid_visible ? "on" : "off");
}

void overlay_render(int width, int height) {
    if (grid_visible) draw_grid(width, height);
    FPS();
    char text[32];
    snprintf(text, sizeof text, "FPS:%.1f", fps);
    void *font = GLUT_BITMAP_HELVETICA_18;
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(width - glutBitmapLength(font, (const unsigned char *)text),
                  height - glutBitmapHeight(font));
    glutBitmapString(font, (const unsigned char *)text);
    glPopAttrib();
}
