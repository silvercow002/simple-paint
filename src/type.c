#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include "type.h"
#include "pen.h"

typedef struct {
    int x, y;
    color_e color;
    char content[256];
} Text;

typedef struct {
    size_t count;
    size_t capacity;
    Text meta[];
} Texts;

static Texts *texts;
static Text typing_text;
static size_t typing_length;
typedef enum {
    TYPE_INACTIVE,
    TYPE_WAITING_POSITION,
    TYPE_EDITING
} TypeState;

static TypeState typing = TYPE_INACTIVE;

void type_init(void) {
    size_t _size = 10000;
    texts = malloc(sizeof *texts + _size * sizeof texts->meta[0]);
    texts->count = 0;
    texts->capacity = _size;
}

void type_cancel(void) {
    typing = TYPE_INACTIVE;
    typing_length = 0;
    glutPostRedisplay();
}

static void apply_color(color_e color) {
    static const GLfloat colors[][3] = {
        {1, 0, 0}, {0, 0, 1}, {0, 1, 0},
        {1, 1, 0}, {0, 0, 0}
    };
    glColor3fv(colors[color]);
}

static void draw_text(const Text *text, int height) {
    apply_color(text->color);
    glRasterPos2i(text->x, height - text->y);
    for (const char *c = text->content; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (unsigned char)*c);
}

void type_begin(void) {
    type_cancel();
    typing = TYPE_WAITING_POSITION;
    fprintf(stderr, "start type\n");
}

void type_commit(void) {
    if (typing == TYPE_EDITING && typing_length) {
        if (texts->count >= texts->capacity) {
            fprintf(stderr, "texts reached capacity\n");
            return;
        }
        texts->meta[texts->count++] = typing_text;
    }
    typing = TYPE_INACTIVE;
    typing_length = 0;
    glutPostRedisplay();
}

int type_keyboard(unsigned char key) {
    if (typing == TYPE_INACTIVE) return 0;

    // enter to commit
    // esc to cancel
    if (key == 27) {
        type_cancel();
    } else if (key == 13) {
        type_commit();
    } else if (typing == TYPE_EDITING) {
        if (key == 8 || key == 127) {
            if (typing_length) typing_text.content[--typing_length] = '\0';
        } else if (key >= 32 && key <= 126 &&
                   typing_length < sizeof typing_text.content - 1) {
            typing_text.content[typing_length++] = (char)key;
            typing_text.content[typing_length] = '\0';
        }
    }
    glutPostRedisplay();
    return 1;
}

void type_preview(int height) {
    if (typing == TYPE_EDITING) draw_text(&typing_text, height);
}

int type_mouse(int button, int state, int x, int y) {
    if (typing == TYPE_INACTIVE || button != GLUT_LEFT_BUTTON) return 0;
    if (state == GLUT_UP) return 1;
    if (typing != TYPE_INACTIVE) {
        if (typing == TYPE_WAITING_POSITION) {
            typing_text = (Text){x, y, pnt_color, ""};
            typing_length = 0;
            typing = TYPE_EDITING;
        } else {
            typing_text.x = x;
            typing_text.y = y;
        }
        glutPostRedisplay();
        return 1;
    }

    return 0;
}

void type_render(int height) {
    for (size_t i = 0; i < texts->count; ++i)
        draw_text(&texts->meta[i], height);
}

void type_clear_saved(void) {
    texts->count = 0;
}
